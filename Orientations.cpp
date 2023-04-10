#include "Project1/HeaderFiles/Patches.h"//includes other headers within it i.e Colors, EnumClasses.h, Friends.h, GeneralFunctions.h, NamespaceAdditions.h, and Properties.h
#include "Project1/HeaderFiles/Surr4.h"
#include "Project1/HeaderFiles/Orientations.h"

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

void Orientations::Create(Surr4& S)
{
	Create_Orientations(S);
}

OrientationType Orientations::Get_Orientation(pair<int, int> Element, const int LineGroup)
{
	if (LineGroupIsInvalid(LineGroup))
	{
		return Invalid;
	}

	auto it = LineGroup_Element_Orientation[LineGroup].find(Element);
	if (it != LineGroup_Element_Orientation[LineGroup].end())
	{
		return it->second;
	}
	else
	{
		return Invalid;
	}
}

void Orientations::Create_Orientations(Surr4& surr)
{
	const unordered_map<int, unordered_map<pair<int, int>, bitset<4>>>& LineGroup_Element_4Code = surr.Get_4Code();

	for (auto linegroup : LineGroup_Element_4Code)
	{
		const int LineGroup = linegroup.first;

		for (auto refelement : linegroup.second)
		{
			const pair<int, int> RefElement = refelement.first;
			bitset<4> Code4 = refelement.second;

			OrientationType Orientation = Get_Orientation(Code4);

			if (ElementIsValid(Orientation, LineGroup))
			{
				LineGroup_Element_Orientation[LineGroup][RefElement] = Orientation;
			}
		}
	}
}

const unordered_map<int, unordered_map<pair<int, int>, OrientationType>>& Orientations::Get_Orientation() const
{
	return LineGroup_Element_Orientation;
}

OrientationType Orientations::Get_Orientation(bitset<4> Code4)
{

	if (CodeIsInvalid(Code4))
	{
		return Invalid;
	}

	if ((Code4[0] == Code4[3]) && (Code4[1] == Code4[2]))
		return Vertical;

	else if ((Code4[1] == Code4[3]) && (Code4[0] != Code4[2]))
		return NEDiagonal;

	else if ((Code4[0] == Code4[1]) && (Code4[2] == Code4[3]))
		return Horizontal;

	else if ((Code4[0] == Code4[2]) && (Code4[1] != Code4[3]))
		return NWDiagonal;
}

bool Orientations::CodeIsInvalid(bitset<4> Code4)
{
	return Code4 == bitset<4>("1111") && Code4 == bitset<4>("0000");
}

bool Orientations::ElementIsValid(const OrientationType Orientation, const int LineGroup)
{
	if (LineGroupIsInvalid(LineGroup))
	{
		throw logic_error("Linegroup is invalid");
	}

	if (Orientation == Horizontal || Orientation == Vertical)
	{
		return true;
	}

	if (Orientation == NEDiagonal)
	{
		return LineGroup != 0 && LineGroup != 2;
	}

	if (Orientation == NWDiagonal)
	{
		return LineGroup != 1 && LineGroup != 3;
	}
}

bool Orientations::LineGroupIsInvalid(const int LineGroup)
{
	return LineGroup < 0 || LineGroup > 3;
}