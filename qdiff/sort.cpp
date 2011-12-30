#include "sort.h"

using namespace qdiff;

Sort::Sort(void)
{
}

Sort::~Sort(void)
{
}

void Sort::quickSort(std::vector<fs::path>* ay)
{
  quickSort(ay, 0, ay->size() - 1);
}

// QuickSort implementation
void Sort::quickSort(std::vector<fs::path>* ay, int lower, int upper)
{
	// Check for non-base case
	if (lower < upper)
	{
		// Split and Sort partitions
		int split = partition(ay, lower, upper);
		quickSort(ay, lower, split - 1);
		quickSort(ay, split + 1, upper);
	}
}

// QuickSort partition implementation
int Sort::partition(std::vector<fs::path>* ay, int lower, int upper)
{
  // Pivot with first element
	int left = lower + 1;
  fs::path pivot = (*ay)[lower];
	int right = upper;

	// Partition array elements
  fs::path swap;
	while (left <= right)
	{
		// Find item out of place
		while (left <= right)
		{
      fs::path fetch = (*ay)[left];
			if (fetch.leaf() > pivot.leaf())
				break;
			left = left + 1;
		}
		while (left <= right)
		{
      fs::path fetch = (*ay)[right];
			if (fetch.leaf() <= pivot.leaf())
				break;
			right = right - 1;
		}

		// Swap values if necessary
		if (left < right)
		{
			swap = (*ay)[left];
      (*ay)[left] = (*ay)[right];
      (*ay)[right] = swap;
			left = left + 1;
			right = right - 1;
		}
	}

	// Move pivot element
	swap = (*ay)[lower];
	(*ay)[lower] = (*ay)[right];
	(*ay)[right] = swap;

	return right;
}
