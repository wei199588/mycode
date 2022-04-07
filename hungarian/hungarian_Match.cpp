#include "hungarian_Match.h"
#include <iostream>

HungarianMatch::HungarianMatch(const std::vector<std::vector<double>> &costs)
    : matrix_size_(0), costs_(), max_cost_(0), rows_covered_(), cols_covered_(), marks_(), stars_in_col_(),
      preimage_(), image_(), zero_col_(0), zero_row_(0), width_(0), height_(0), state_(NULL)
{
    width_ = costs.size();

    if (width_ > 0)
    {
        height_ = costs[0].size();
    }
    else
    {
        height_ = 0;
    }

    matrix_size_ = std::max(width_, height_);
    max_cost_ = 0;

    costs_.resize(matrix_size_);
    for (int row = 0; row < matrix_size_; ++row)
    {
        costs_[row].resize(matrix_size_);
    }

    for (int row = 0; row < matrix_size_; ++row)
    {
        for (int col = 0; col < matrix_size_; ++col)
        {
            if ((row >= width_) || (col >= height_))
            {
                costs_[row][col] = 0;
            }
            else
            {
                costs_[row][col] = costs[row][col];
                max_cost_ = std::max(max_cost_, costs_[row][col]);
            }
        }
    }

    marks_.resize(matrix_size_);
    for (int row = 0; row < matrix_size_; ++row)
    {
        marks_[row].resize(matrix_size_);
        for (int col = 0; col < matrix_size_; ++col)
        {
            marks_[row][col] = NONE;
        }
    }

    stars_in_col_.resize(matrix_size_);

    rows_covered_.resize(matrix_size_);
    cols_covered_.resize(matrix_size_);

    preimage_.resize(matrix_size_ * 2);
    image_.resize(matrix_size_ * 2);
}

void HungarianMatch::Maximize(std::vector<int> *preimage, std::vector<int> *image)
{
    for (int row = 0; row < width_; ++row)
    {
        for (int col = 0; col < height_; ++col)
        {
            costs_[row][col] = max_cost_ - costs_[row][col];
        }
    }

    Minimize(preimage, image);
}

void HungarianMatch::Minimize(std::vector<int> *preimage, std::vector<int> *image)
{
    DoMunkres();
    FindAssignments(preimage, image);
}

void HungarianMatch::FindAssignments(std::vector<int> *preimage, std::vector<int> *image)
{
    preimage->clear();
    image->clear();
    for (int row = 0; row < width_; ++row)
    {
        for (int col = 0; col < height_; ++col)
        {
            if (IsStarred(row, col))
            {
                preimage->push_back(row);
                image->push_back(col);
                break;
            }
        }
    }
}

int HungarianMatch::FindStarInRow(int row) const
{
    for (int col = 0; col < matrix_size_; ++col)
    {
        if (IsStarred(row, col))
        {
            return col;
        }
    }

    return kHungarianMatchColNotFound;
}

int HungarianMatch::FindStarInCol(int col) const
{
    if (!ColContainsStar(col))
    {
        return kHungarianMatchRowNotFound;
    }

    for (int row = 0; row < matrix_size_; ++row)
    {
        if (IsStarred(row, col))
        {
            return row;
        }
    }

    return kHungarianMatchRowNotFound;
}

int HungarianMatch::FindPrimeInRow(int row) const
{
    for (int col = 0; col < matrix_size_; ++col)
    {
        if (IsPrimed(row, col))
        {
            return col;
        }
    }

    return kHungarianMatchColNotFound;
}

void HungarianMatch::ClearPrimes()
{
    for (int row = 0; row < matrix_size_; ++row)
    {
        for (int col = 0; col < matrix_size_; ++col)
        {
            if (IsPrimed(row, col))
            {
                marks_[row][col] = NONE;
            }
        }
    }
}

void HungarianMatch::ClearCovers()
{
    for (int x = 0; x < matrix_size_; x++)
    {
        UncoverRow(x);
        UncoverCol(x);
    }
}

double HungarianMatch::FindSmallestUncovered() const
{
    double minval = std::numeric_limits<double>::max();

    for (int row = 0; row < matrix_size_; ++row)
    {
        if (RowCovered(row))
        {
            continue;
        }

        for (int col = 0; col < matrix_size_; ++col)
        {
            if (ColCovered(col))
            {
                continue;
            }

            minval = std::min(minval, costs_[row][col]);
        }
    }

    return minval;
}

bool HungarianMatch::FindZero(int *zero_row, int *zero_col) const
{
    for (int row = 0; row < matrix_size_; ++row)
    {
        if (RowCovered(row))
        {
            continue;
        }

        for (int col = 0; col < matrix_size_; ++col)
        {
            if (ColCovered(col))
            {
                continue;
            }

            if (costs_[row][col] == 0)
            {
                *zero_row = row;
                *zero_col = col;
                return true;
            }
        }
    }

    return false;
}

void HungarianMatch::PrintMatrix()
{
    for (int row = 0; row < matrix_size_; ++row)
    {
        for (int col = 0; col < matrix_size_; ++col)
        {
            printf("%g ", costs_[row][col]);

            if (IsStarred(row, col))
            {
                printf("*");
            }

            if (IsPrimed(row, col))
            {
                printf("'");
            }
        }
        
        printf("\n");
    }
}

void HungarianMatch::DoMunkres()
{
    int max_iter = 1000;
    int iter_num = 0;
    state_ = &HungarianMatch::ReduceRows;
    while (state_ != NULL && iter_num < max_iter)
    {
        (this->*state_)();
        ++iter_num;
    }

    if (iter_num >= max_iter)
    {
        CheckStar();
    }
}

void HungarianMatch::CheckStar()
{
    for (int row = 0; row < width_; ++row)
    {
        int star_col = -1;
        bool is_single = true;
        for (int col = 0; col < height_; ++col)
        {
            if (IsStarred(row, col))
            {
                if (star_col == -1)
                {
                    star_col = col;
                }
                else
                {
                    is_single = false;
                    break;
                }
            }
        }
        if (!is_single)
        {
            for (int col = 0; col < height_; ++col)
            {
                UnStar(row, col);
            }
        }
    }
}

void HungarianMatch::ReduceRows()
{
    for (int row = 0; row < matrix_size_; ++row)
    {
        double min_cost = costs_[row][0];
        for (int col = 1; col < matrix_size_; ++col)
        {
            min_cost = std::min(min_cost, costs_[row][col]);
        }

        for (int col = 0; col < matrix_size_; ++col)
        {
            costs_[row][col] -= min_cost;
        }
    }

    state_ = &HungarianMatch::StarZeroes;
}

void HungarianMatch::StarZeroes()
{
    for (int row = 0; row < matrix_size_; ++row)
    {
        if (RowCovered(row))
        {
            continue;
        }

        for (int col = 0; col < matrix_size_; ++col)
        {
            if (ColCovered(col))
            {
                continue;
            }

            if (costs_[row][col] == 0)
            {
                Star(row, col);
                CoverRow(row);
                CoverCol(col);
                break;
            }
        }
    }

    ClearCovers();
    state_ = &HungarianMatch::CoverStarredZeroes;
}

void HungarianMatch::CoverStarredZeroes()
{
    int num_covered = 0;

    for (int col = 0; col < matrix_size_; ++col)
    {
        if (ColContainsStar(col))
        {
            CoverCol(col);
            num_covered++;
        }
    }

    if (num_covered >= matrix_size_)
    {
        state_ = NULL;
        return;
    }

    state_ = &HungarianMatch::PrimeZeroes;
}

void HungarianMatch::PrimeZeroes()
{
    for (;;)
    {
        int zero_row, zero_col;
        if (!FindZero(&zero_row, &zero_col))
        {
            // No uncovered zeroes.
            state_ = &HungarianMatch::AugmentPath;
            return;
        }

        Prime(zero_row, zero_col);
        int star_col = FindStarInRow(zero_row);

        if (star_col != kHungarianMatchColNotFound)
        {
            CoverRow(zero_row);
            UncoverCol(star_col);
        }
        else
        {
            preimage_[0] = zero_row;
            image_[0] = zero_col;
            state_ = &HungarianMatch::MakeAugmentingPath;
            return;
        }
    }
}

void HungarianMatch::MakeAugmentingPath()
{
    bool done = false;
    int count = 0;

    while (!done)
    {
        // First construct the alternating path...
        int row = FindStarInCol(image_[count]);

        if (row != kHungarianMatchRowNotFound)
        {
            count++;
            preimage_[count] = row;
            image_[count] = image_[count - 1];
        }
        else
        {
            done = true;
        }

        if (!done)
        {
            int col = FindPrimeInRow(preimage_[count]);
            count++;
            preimage_[count] = preimage_[count - 1];
            image_[count] = col;
        }
    }

    for (int i = 0; i <= count; ++i)
    {
        int row = preimage_[i];
        int col = image_[i];

        if (IsStarred(row, col))
        {
            UnStar(row, col);
        }
        else
        {
            Star(row, col);
        }
    }

    ClearCovers();
    ClearPrimes();

    state_ = &HungarianMatch::CoverStarredZeroes;
}

void HungarianMatch::AugmentPath()
{
    double minval = FindSmallestUncovered();

    for (int row = 0; row < matrix_size_; ++row)
    {
        for (int col = 0; col < matrix_size_; ++col)
        {
            if (RowCovered(row))
            {
                costs_[row][col] += minval;
            }

            if (!ColCovered(col))
            {
                costs_[row][col] -= minval;
            }
        }
    }

    state_ = &HungarianMatch::PrimeZeroes;
}