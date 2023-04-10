//Other headers have been implicityly included in this file. They include EnumClasses.h, Friends.h, GeneralFunctions.h, NamespaceStdAdditions.h, Patches.h, and Properties.h
#include "Project1/HeaderFiles/Colors.h"
#include "Project1/HeaderFiles/Patches.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "iostream"
#include <vector>
#include <string>
#include <map>
#include <list>
#include <numeric>
#include <limits>
#include <deque>
#include <algorithm>
#include <utility>
#include <set>
#include <tuple>
#include <bitset>
#include <stack>
#include <type_traits>
#include <unordered_set>
#include <unordered_map>

void Patches::Create(Mat& Img)
{
	Colors::Create(Img);

	int StartOfSimilarityType = static_cast<int>(SimilarityType::TTT);
	int EndOfSimilarityType = static_cast<int>(SimilarityType::FFF);
	for (int i = StartOfSimilarityType; i <= EndOfSimilarityType; ++i)
	{
		SimilarityType CurrentSimilarityType = static_cast<SimilarityType>(i);

		unordered_map<pair<int, int>, int> element_patchindex;
		unordered_map<int, vector<pair<int, int>>> patchindex_patchmembers;

		Create_Patches(element_patchindex, patchindex_patchmembers, CurrentSimilarityType);

		Add_Patches(element_patchindex, patchindex_patchmembers);
	}
}

int Patches::Get_PatchIndex(const pair<int, int> Element)
{
	auto it = Element_PatchIndex.find(Element);
	if (it != Element_PatchIndex.end())
	{
		return it->second;
	}

	else
	{
		return -1;
	}
}

const int Patches::Get_PatchIndex(const pair<int, int> Element) const
{
	auto it = Element_PatchIndex.find(Element);
	if (it != Element_PatchIndex.end())
	{
		return it->second;
	}

	else
	{
		return -1;
	}
}

vector<int> Patches::Get_PatchIndexes(const vector<pair<int, int>>& Elements)
{
	vector<int> PatchIndexes;
	for (auto Element : Elements)
	{
		PatchIndexes.push_back(Get_PatchIndex(Element));
	}

	return PatchIndexes;
}

vector<pair<int, int>> Patches::Get_PatchMembers(const int PatchIndex)
{
	auto it = PatchIndex_PatchMembers.find(PatchIndex);
	if (it != PatchIndex_PatchMembers.end())
	{
		return it->second;
	}
	else
	{
		return vector<pair<int, int>>();
	}
}

vector<pair<int, int>> Patches::Get_PatchMembers(const pair<int, int>& Element)
{
	if (isInImage(Element))
	{
		int PatchIndex = Element_PatchIndex[Element];
		return PatchIndex_PatchMembers[PatchIndex];
	}
}

bool Patches::PatchIndexIsSame(const  pair<int, int> RefElement, const pair<int, int> Element)
{
	if (isInImage(RefElement) && isInImage(Element))
	{
		return (Get_PatchIndex(RefElement) == Get_PatchIndex(Element));
	}

	return false;
}

bool Patches::PatchIndexIsSame(const int PatchIndex, const pair<int, int> Element)
{
	if (isInImage(Element))
	{
		return PatchIndex == Get_PatchIndex(Element);
	}

	return false;
}

bool Patches::PatchIndexIsSame(const int PatchIndex, const pair<int, int> Element)const
{
	if (isInImage(Element))
	{
		return PatchIndex == Get_PatchIndex(Element);
	}

	return false;
}

Mat Patches::Get_Img() { return Img; }

void Patches::Create_Patches(unordered_map<pair<int, int>, int>& element_patchindex, unordered_map<int, vector<pair<int, int>>>& patchindex_patchmembers, SimilarityType CurrentSimilarityType)
{
	for (int i = 0; i != Img.rows; ++i)
	{
		for (int j = 0; j != Img.cols; ++j)
		{
			const pair<int, int> CurrentElement = make_pair(j, i);
			if (!IsInAPatchOfAnotherSimilarityType(CurrentElement))
			{
				const vector<pair<int, int>> NWNeighbours = Get_NWNeighbours(CurrentElement);

				bool CurrentElementHasAPatch = false;
				for (auto NeighbouringElement : NWNeighbours)
				{
					if (!IsInAPatchOfAnotherSimilarityType(NeighbouringElement))
					{
						int NeighborPatchIndex;
						if (isInImage(NeighbouringElement)) NeighborPatchIndex = Get_PatchIndex(element_patchindex, NeighbouringElement);
						else continue;

						if (NeighborPatchIndex == -1) throw "Neighbor has no patch";//since the neighbors have already been dealth with, they ought to have patches

						if (ColorIsSame(CurrentSimilarityType, CurrentElement, NeighbouringElement))//ColorIsSame function is defined under Colors.h
						{
							if (CurrentElementHasAPatch)
							{
								int EmptyPatch = Relocate_PatchMembers(element_patchindex, patchindex_patchmembers, element_patchindex[CurrentElement], NeighborPatchIndex);
								if (EmptyPatch != -1)//Elements were moved from one patch to another. Therefore, there is an empty patch
								{
									int LastPatchIndex = patchindex_patchmembers.size();
									EmptyPatch = Relocate_PatchMembers(element_patchindex, patchindex_patchmembers, EmptyPatch, LastPatchIndex);
									patchindex_patchmembers.erase(LastPatchIndex);//The last patch will always end up empty
								}
							}

							else
							{
								//Add the current element as a member to the patch for which it's similar neighbours are
								element_patchindex[CurrentElement] = NeighborPatchIndex;
								patchindex_patchmembers[NeighborPatchIndex].push_back(CurrentElement);
								CurrentElementHasAPatch = true;
							}
						}
					}
				}

				if (!CurrentElementHasAPatch) Start_ANewPatch(element_patchindex, patchindex_patchmembers, CurrentElement);
			}
		}
	}
}

bool Patches::IsInPatches(const int PatchIndex)
{
	auto it = PatchIndex_PatchMembers.find(PatchIndex);

	return it != PatchIndex_PatchMembers.end();
}

bool Patches::IsInAPatchOfAnotherSimilarityType(pair<int, int> Element)
{
	return Element_PatchIndex.find(Element) != Element_PatchIndex.end();//Note E and P of the variable Elemenent_PatchIndex are in caps. This is the class variable as opposed to element_patchindex that are declared inside the create function
}

void Patches::Start_ANewPatch(unordered_map<pair<int, int>, int>& element_patchindex, unordered_map<int, vector<pair<int, int>>>& patchindex_patchmembers, pair<int, int> CurrentElement)
{
	int NewPatchIndex = static_cast<int>(patchindex_patchmembers.size());
	element_patchindex[CurrentElement] = NewPatchIndex;
	patchindex_patchmembers[NewPatchIndex].reserve(1000);
	patchindex_patchmembers[NewPatchIndex].push_back(CurrentElement);
}

int Patches::Get_PatchIndex(unordered_map<pair<int, int>, int> element_patchindex, const pair<int, int> Element)
{
	auto it = element_patchindex.find(Element);
	if (it != element_patchindex.end())
	{
		return it->second;
	}

	else
	{
		return -1;
	}
}

int Patches::Relocate_PatchMembers(unordered_map<pair<int, int>, int>& element_patchindex, unordered_map<int, vector<pair<int, int>>>& patchindex_patchmembers, int PatchIndex1, int PatchIndex2)
{
	if (PatchIndex1 == PatchIndex2)
	{
		return -1;
	}

	if (PatchIndex2 < PatchIndex1)
	{
		std::swap(PatchIndex1, PatchIndex2);//So we can move members from the members with ther larger PatchIndex to those with lower PatchIndex
	}

	for (auto Element : patchindex_patchmembers[PatchIndex2])
	{
		element_patchindex[Element] = PatchIndex1;
	}

	patchindex_patchmembers[PatchIndex1] = std::move(patchindex_patchmembers[PatchIndex2]);

	return PatchIndex2;
}

vector<pair<int, int>> Patches::Get_NWNeighbours(const pair<int, int> Element)
{
	return
	{
		{ make_pair((Element.first - 1), Element.second) },
		{ make_pair((Element.first - 1), (Element.second - 1)) },
		{ make_pair(Element.first, (Element.second - 1)) },
		{ make_pair((Element.first + 1), (Element.second - 1)) },
	};
}

//Removes patches that only have one element. throws an exception when a patch with no members has been encountered
void Patches::Add_Patches(unordered_map<pair<int, int>, int>& element_patchindex, unordered_map<int, vector<pair<int, int>>>& patchindex_patchmembers)
{
	for (auto it = patchindex_patchmembers.begin(); it != patchindex_patchmembers.end(); ++it)
	{
		size_t CurrentPatchSize = it->second.size();
		if (CurrentPatchSize == 0)//Just so we can handle the three scenarios, when CarrentPatchSize = 0, 1, and more than 1. It also helps us address mistaken deletions of patchmembers in the previous functions
		{
			throw "Class: Patches, Function: Remove_InvalidPatches, Error: Patch with no member!";
		}

		else if (CurrentPatchSize > 1)
		{
			vector<pair<int, int>>& CurrentPatchMembers = it->second;
			int NewPatchIndex = PatchIndex_PatchMembers.size();

			for (auto Element : CurrentPatchMembers)
			{
				Element_PatchIndex[Element] = NewPatchIndex;
			}
			PatchIndex_PatchMembers[NewPatchIndex] = std::move(CurrentPatchMembers);
		}
	}
}