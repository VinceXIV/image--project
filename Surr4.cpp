#include "Project1/HeaderFiles/Surr4.h"
#include "Project1/HeaderFiles/Patches.h"//includes other headers within it i.e Colors, EnumClasses.h, Friends.h, GeneralFunctions.h, NamespaceAdditions.h, and Properties.h

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
#include <random>
#include <bitset>

using namespace std;
using namespace cv;

void Surr4::Create(Patches & patches)
{
	Create_Element4Code(patches);
}

bitset<4> Surr4::Get_4Code(pair<int, int> Element, const int LineGroup)
{
	if (LineGroupIsValid(LineGroup))
	{
		auto it = LineGroup_Element_4Code[LineGroup].find(Element);
		if (it != LineGroup_Element_4Code[LineGroup].end())
		{
			return it->second;
		}
		else
		{
			return bitset<4>("1111");
		}
	}

	else
	{
		throw "LineGroup is not Valid!";
	}
}

const unordered_map<int, unordered_map<pair<int, int>, bitset<4>>>& Surr4::Get_4Code()const { return LineGroup_Element_4Code; }

vector<pair<int, int>> Surr4::Get_4Surr(const int Linegroup, const pair<int, int> Element)
{
	if (LineGroupIsValid(Linegroup))
	{
		auto it = LineGroup_Element_Surr4Elements[Linegroup].find(Element);
		if (it != LineGroup_Element_Surr4Elements[Linegroup].end())
		{
			return it->second;
		}
		else
		{
			return vector<pair<int, int>>();
		}
	}

	else
	{
		return vector<pair<int, int>>();
	}
}

const Friends<pair<int, int>>& Surr4::Get_Friends()const { return ElementFriends; }

Friends<pair<int, int>>& Surr4::Get_Friends() { return ElementFriends; }

const unordered_map<int, unordered_set<pair<int, int>>>& Surr4::Get_Surr4Elements() { return LineGroup_Elements; }

bool Surr4::IsInLineGroup(const pair<int, int> Element, const int LineGroup)
{
	if (LineGroupIsValid(LineGroup))
	{
		return (LineGroup_Elements[LineGroup].end() != LineGroup_Elements[LineGroup].find(Element));
	}

	return false;
}

void Surr4::Get_EdgeElements(vector<pair<int, int>>& EdgeElements)
{
	ElementFriends.Get_friends(EdgeElements);
}

void Surr4::Get_EdgeElements(unordered_set<pair<int, int>>& EdgeElements)
{
	ElementFriends.Get_friends(EdgeElements);
}

void Surr4::Create_Element4Code(Patches& patches)
{
	const Mat& Img = patches.Get_Img();

	for (int Row = 1; Row != (Img.rows - 1); ++Row)
	{
		for (int Col = 1; Col != (Img.cols - 1); ++Col)
		{
			pair<int, int> RefElement = make_pair(Col, Row);

			vector<pair<int, int>> Surr4 = Get_4Surr(Col, Row);

			bitset<4> Bit4 = Create_4Code(Surr4, RefElement, patches);

			//We only need edges in this case. That is, we only need transitions. Therefore, if the four elements in Surr4 belong to the same Patch,
			//they don't prove transition and are, therefore, done away with.
			if ((Bit4 != bitset<4>("1111")) && (Bit4 != bitset<4>("0000")))
			{
				int LineGroup = 0;
				for (auto Element : Surr4)
				{
					LineGroup_Element_4Code[LineGroup][Element] = Bit4;
					LineGroup_Element_Surr4Elements[LineGroup][Element] = Surr4;

					ElementFriends.Make_MutualFriends(Surr4);//Make_MutualFriends is a function in the Friends Class that is declared under the Friends.h

					LineGroup_Elements[LineGroup].insert(Element);

					++LineGroup;
				}
			}
		}
	}

	return;
}

bool Surr4::LineGroupIsValid(const int LineGroup) { return (LineGroup >= 0 && LineGroup < 4); }

vector<pair<int, int>> Surr4::Get_4Surr(const int Col, const int Row)
{
	return
	{
		{ make_pair((Col - 1), Row) },
		{ make_pair(Col, Row) },
		{ make_pair(Col, (Row + 1)) },
		{ make_pair((Col - 1), (Row + 1)) }
	};
}

bitset<4> Surr4::Create_4Code(const vector<pair<int, int>>& Elements, const pair<int, int> Ref_Element, Patches& patches)//return string showing characteristic comparison values
{
	std::bitset<4> Ret;

	const int Ref_PatchIndex = patches.Get_PatchIndex(Ref_Element);
	int pos = 0;
	for (auto Element : Elements)
	{
		if (pos == 1)//Since the element at position 1 of the vector: Elements is Ref_Element
		{
			Ret.set(pos, true);
		}
		else if (patches.PatchIndexIsSame(Ref_PatchIndex, Element))
		{
			Ret.set(pos, true);
		}
		++pos;
	}

	if ((Ret != bitset<4>("1010")) && (Ret != bitset<4>("0010")))//values are similar to their diagonal counterparts or none of the four values are similar
	{
		return Ret;
	}
	else//for each Elemant patch that is different from Ref_Element's patch, check to to see if it has another element from the similar patch as it
	{
		Ret.reset();
		vector<int> PatchIndexes = patches.Get_PatchIndexes(Elements);

		string S;
		for (size_t i = 0; i != Ret.size(); ++i)
		{
			if (std::count(PatchIndexes.begin(), PatchIndexes.end(), PatchIndexes[i]) > 1)
			{
				Ret.set(pos, true);
			}
		}

		return Ret;
	}
}
