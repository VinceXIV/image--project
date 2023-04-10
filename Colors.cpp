#include "Project1/HeaderFiles/GeneralFunctions.h"
#include "Project1/HeaderFiles/Colors.h"

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

using namespace std;
using namespace cv;

void Colors::Create(Mat& img)
{
	Img = img;
	Create_colors();
	Create_ColorFriends();
}

const unordered_map<size_t, unordered_set<pair<int, int>>>& Colors::Get_Colors() { return ColorCode_Elements; }

size_t Colors::Get_ColorNumber() { return ColorCode_Elements.size(); }

size_t Colors::Get_ColorCode(const pair<int, int> Element) { return Element_ColorCode[Element]; }

//SHOULD THROW AN EXCEPTION IF ERROR
tuple<uchar, uchar, uchar> Colors::Get_Color(const size_t ColorCode)
{
	auto it = ColorCode_Color.find(ColorCode);
	if (it == ColorCode_Color.end()) return make_tuple(0, 0, 0);

	return it->second;
}

//SHOULD THROW AN EXCEPTION IF ERROR
tuple<uchar, uchar, uchar> Colors::Get_Color(const pair<int, int> Element)
{
	if (isInImage(Element))
	{
		size_t ColorCode = Get_ColorCode(Element);
		return Get_Color(ColorCode);
	}

	return make_tuple(0, 0, 0);
}

bool Colors::ColorIsSame(const SimilarityType similaritytype, const pair<int, int> ElementX, const pair<int, int> ElementY)
{
	bool flag = isInImage(ElementX) ? isInImage(ElementY) ? true : false : false;

	if (!flag)
	{
		return false;
	}

	if (flag)
	{
		return similaritytype == Get_ColorType(ElementX, ElementY);
	}

	return false;
}

//SHOULD ALSO THROW AN EXCEPTION IN CASE OF ERROR
SimilarityType Colors::Get_ColorType(const pair<int, int> ElementX, const pair<int, int> ElementY)
{
	bool flag = isInImage(ElementX) ? isInImage(ElementY) ? true : false : false;

	if (!flag)
	{
		throw "One or both of these elements are not part of the Image";
	}

	if (flag)
	{
		tuple<uchar, uchar, uchar> ElementXColor = Get_Color(ElementX);
		tuple<uchar, uchar, uchar> ElementYColor = Get_Color(ElementY);

		//The first value is T
		if (get<0>(ElementXColor) == get<0>(ElementYColor))
		{
			//second one is also T
			if (get<1>(ElementXColor) == get<1>(ElementYColor))
			{
				//Third is T
				if (get<2>(ElementXColor) == get<2>(ElementYColor))
				{
					return TTT;
				}
				else
				{
					return TTF;
				}
			}

			//Second one is F
			else
			{
				//Third is T
				if (get<2>(ElementXColor) == get<2>(ElementYColor))
				{
					return TFT;
				}
				else
				{
					return TFF;
				}
			}
		}

		//First is F
		else
		{
			//Second is T
			if (get<1>(ElementXColor) == get<1>(ElementYColor))
			{
				if (get<2>(ElementXColor) == get<2>(ElementYColor))
				{
					return FTT;
				}
				else
				{
					return FTF;
				}
			}

			else
			{
				if (get<2>(ElementXColor) == get<2>(ElementYColor))
				{
					return FFT;
				}
				else
				{
					return FFF;
				}
			}
		}
	}
}

bool Colors::isInImage(const pair<int, int> Element)
{
	if ((Element.first < 0) || (Element.second < 0) || (Element.first >= Img.cols) || (Element.second >= Img.rows))
	{
		return false;
	}
	return true;
}

void Colors::Remove_NotInImage(list<pair<int, int>>& Container)
{
	auto it = Container.begin();
	while (it != Container.end())
	{
		if (!isInImage(*it))
		{
			it = Container.erase(it);
		}
		else
		{
			++it;
		}
	}
}

const Mat& Colors::Get_Img() { return Img; }

Friends<tuple<uchar, uchar, uchar>, tuple<uchar, uchar, uchar>>& Colors::Get_ColorFriends()
{
	return ColorFriends;
}

const Friends<tuple<uchar, uchar, uchar>, tuple<uchar, uchar, uchar>>& Colors::Get_ColorFriends()const
{
	return ColorFriends;
}


bool Colors::operator== (const Colors& X) { return ColorCode_Elements == X.ColorCode_Elements; }
bool Colors::operator!= (const Colors& X) { return !operator==(X); }
bool Colors::operator < (const Colors& X) { return ColorCode_Color.size() < X.ColorCode_Color.size(); }
bool Colors::operator<= (const Colors& X) { return (operator<(X)) || (operator==(X)); }
bool Colors::operator > (const Colors& X) { return ColorCode_Color.size() > X.ColorCode_Color.size(); }
bool Colors::operator>= (const Colors& X) { return (operator>(X)) || (operator==(X)); }


void Colors::Create_ColorFriends()
{
	for (int Col = 0; Col != Img.cols; Col++)
	{
		for (int Row = 0; Row != Img.rows; Row++)
		{
			const pair<int, int> CurrElement = make_pair(Col, Row);
			tuple<uchar, uchar, uchar> CurrElementColor = Get_Color(CurrElement);

			vector<pair<int, int>> Surr8 = Get_8Surr(Col, Row);
			vector<tuple<uchar, uchar, uchar>> Surr8ElementColors;
			for (auto Element : Surr8)
			{
				if (isInImage(Element))
				{
					Surr8ElementColors.push_back(Get_Color(Element));
				}
			}

			ColorFriends.Make_friends(CurrElementColor, Surr8ElementColors);
			ColorFriends.Make_Property(CurrElementColor, CurrElementColor);
		}

	}
}

void Colors::Create_colors()
{
	Create_ElementColorCode();

	//Create ColorCode_Elements
	for (auto it = Element_ColorCode.begin(); it != Element_ColorCode.end(); ++it)
	{
		pair<int, int> Curr_Element = it->first;
		size_t Curr_ColorCode = it->second;

		ColorCode_Elements[Curr_ColorCode].insert(std::move(Curr_Element));
	}
}

void Colors::Create_ElementColorCode()
{
	for (int Col = 0; Col != Img.cols; ++Col)
	{
		for (int Row = 0; Row != Img.rows; ++Row)
		{
			pair<int, int> Curr_Element = make_pair(Col, Row);
			uchar Blue = Img.at<Vec3b>(Col, Row)[0];
			uchar Green = Img.at<Vec3b>(Col, Row)[1];
			uchar Red = Img.at<Vec3b>(Col, Row)[2];

			tuple<uchar, uchar, uchar> Color = std::move(make_tuple(Blue, Green, Red));
			size_t ColorCode = (((get<0>(Color) * 255) + get<1>(Color)) * 255) + get<2>(Color);

			Element_ColorCode[Curr_Element] = std::move(ColorCode);
			ColorCode_Color[ColorCode] = std::move(Color);
		}
	}
}

void Colors::Add_SizesToColorFriends()
{
	unordered_map<tuple<uchar, uchar, uchar>, unordered_set<tuple<uchar, uchar, uchar>>>& Color_ItsFriends = ColorFriends.Get();

	for (auto color : Color_ItsFriends)
	{
		const tuple<uchar, uchar, uchar> Color = color.first;

		ColorFriends.Make_Property(Color, Color);
	}
}