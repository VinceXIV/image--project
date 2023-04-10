#include "Project1/HeaderFiles/GeneralFunctions.h"
#include "Project1/HeaderFiles/NamespaceStdAdditions.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <set>
#include <unordered_set>

using namespace std;
using namespace cv;

Side Get_Side(const pair<int, int> Base, const pair<int, int> Ele)
{
	std::pair<int, int> X = std::make_pair((Base.first - Ele.first), (Base.second - Ele.second));
	int Test = abs(X.first) - abs(X.second);

	if (!(X.first) && !(X.second)) return NA;//Base is same as Ele

	if ((X.first > 0) && (X.second > 0))
	{
		if (Test > 0) return NWW;
		else if (Test < 0) return NNW;
		else if (Test == 0) return NW;
	}
	else if ((X.second > 0) && (X.first == 0)) return N;

	else if ((X.first < 0) && (X.second > 0))
	{
		if (Test < 0) return NNE;
		else if (Test > 0) return NEE;
		else if (Test == 0) return NE;
	}
	else if ((X.first < 0) && (X.second == 0)) return E;

	else if ((X.first < 0) && (X.second < 0))
	{
		if (Test > 0) return SEE;
		else if (Test < 0) return SSE;
		else if (Test == 0) return SE;
	}
	else if ((X.second < 0) && (X.first == 0)) return S;

	else if ((X.first > 0) && (X.second < 0))
	{
		if (Test > 0) return SWW;
		else if (Test < 0) return SSW;
		else if (Test == 0) return SW;
	}
	else // ((X.first > 0) && (X.second == 0)) 
		return W;
}

double Get_Distance(const pair<int, int>& X, const pair<int, int>& Y)
{
	return sqrt((pow((X.first - Y.first), 2)) + (pow((X.second - Y.second), 2)));
}

std::vector<pair<int, int>> Get_8Surr(const int a, const int b)
{
	return
	{
		{ make_pair((a - 1), (b - 1)) },
		{ make_pair(a, (b - 1)) },
		{ make_pair((a + 1), (b - 1)) },
		{ make_pair((a + 1), b) },
		{ make_pair((a + 1), (b + 1)) },
		{ make_pair(a, (b + 1)) },
		{ make_pair((a - 1), (b + 1)) },
		{ make_pair((a - 1), b) }
	};
}

vector<pair<int, int>> Get_Surr(const pair<int, int> Ele, const Side side)
{
	switch (side)
	{
	case NW:
		return { { make_pair((Ele.first - 1), Ele.second) },{ make_pair((Ele.first - 1), (Ele.second - 1)) },{ make_pair((Ele.first), (Ele.second - 1)) } };
	case NNW:
		return { { make_pair((Ele.first - 1), Ele.second - 1) },{ make_pair((Ele.first), (Ele.second - 1)) },{ make_pair((Ele.first + 1), (Ele.second - 1)) } };
	case N:
		return { { make_pair((Ele.first - 1), Ele.second - 1) },{ make_pair((Ele.first), (Ele.second - 1)) },{ make_pair((Ele.first + 1), (Ele.second - 1)) } };
	case NNE:
		return { { make_pair((Ele.first - 1), Ele.second - 1) },{ make_pair((Ele.first), (Ele.second - 1)) },{ make_pair((Ele.first + 1), (Ele.second - 1)) } };
	case NE:
		return { { make_pair((Ele.first), Ele.second - 1) },{ make_pair((Ele.first + 1), (Ele.second - 1)) },{ make_pair((Ele.first + 1), (Ele.second)) } };
	case NEE:
		return { { make_pair((Ele.first + 1), Ele.second - 1) },{ make_pair((Ele.first + 1), (Ele.second)) },{ make_pair((Ele.first + 1), (Ele.second + 1)) } };
	case E:
		return { { make_pair((Ele.first + 1), Ele.second - 1) },{ make_pair((Ele.first + 1), (Ele.second)) },{ make_pair((Ele.first + 1), (Ele.second + 1)) } };
	case SEE:
		return { { make_pair((Ele.first + 1), Ele.second - 1) },{ make_pair((Ele.first + 1), (Ele.second)) },{ make_pair((Ele.first + 1), (Ele.second + 1)) } };
	case SE:
		return { { make_pair((Ele.first + 1), Ele.second) },{ make_pair((Ele.first + 1), (Ele.second + 1)) },{ make_pair((Ele.first), (Ele.second + 1)) } };
	case SSE:
		return { { make_pair((Ele.first + 1), Ele.second + 1) },{ make_pair((Ele.first), (Ele.second + 1)) },{ make_pair((Ele.first - 1), (Ele.second + 1)) } };
	case S:
		return { { make_pair((Ele.first + 1), Ele.second + 1) },{ make_pair((Ele.first), (Ele.second + 1)) },{ make_pair((Ele.first - 1), (Ele.second + 1)) } };
	case SSW:
		return { { make_pair((Ele.first + 1), Ele.second + 1) },{ make_pair((Ele.first), (Ele.second + 1)) },{ make_pair((Ele.first - 1), (Ele.second + 1)) } };
	case SW:
		return { { make_pair((Ele.first), Ele.second + 1) },{ make_pair((Ele.first - 1), (Ele.second + 1)) },{ make_pair((Ele.first - 1), (Ele.second)) } };
	case SWW:
		return { { make_pair((Ele.first - 1), Ele.second + 1) },{ make_pair((Ele.first - 1), (Ele.second)) },{ make_pair((Ele.first - 1), (Ele.second - 1)) } };
	case W:
		return { { make_pair((Ele.first - 1), Ele.second + 1) },{ make_pair((Ele.first - 1), (Ele.second)) },{ make_pair((Ele.first - 1), (Ele.second - 1)) } };
	case NWW:
		return { { make_pair((Ele.first - 1), Ele.second + 1) },{ make_pair((Ele.first - 1), (Ele.second)) },{ make_pair((Ele.first - 1), (Ele.second - 1)) } };
	case NA:
		return Get_8Surr(Ele.first, Ele.second);
	}
}

list<pair<int, int>> GetSurr(const pair<int, int> Base, const list<pair<int, int>>& Querry)
{
	list<pair<int, int>> Ret;

	if (Querry.empty())
	{
		Ret.push_back(Base);

		return Ret;
	}

	for(auto ele: Querry)
	{
		Side side = Get_Side(Base, ele);
		vector<pair<int, int>> Surr = Get_Surr(ele, side);

		//SubRet will remove repititions
		unordered_set<pair<int, int>> SubRet(Surr.begin(), Surr.end());

		std::move(SubRet.begin(), SubRet.end(), back_inserter(Ret));
	}

	return Ret;
}

unsigned int Get_Difference(const int EleXSize, const int EleYSize)
{
	return abs(EleXSize - EleYSize);
}

unsigned int Get_Difference(const pair<int, int> EleXSize, const pair<int, int> EleYSize)
{
	int A = EleXSize.first - EleYSize.first;
	int B = EleXSize.second - EleYSize.second;

	return static_cast<unsigned int>(sqrt((pow(A, 2)) + (pow(B, 2))));
}

unsigned int Get_Difference(const tuple<uchar, uchar, uchar> EleXSize, const tuple<uchar, uchar, uchar> EleYSize)
{
	int A = static_cast<int>(get<0>(EleXSize)) - static_cast<int>(get<0>(EleYSize));
	int B = static_cast<int>(get<1>(EleXSize)) - static_cast<int>(get<1>(EleYSize));
	int C = static_cast<int>(get<2>(EleXSize)) - static_cast<int>(get<2>(EleYSize));

	return static_cast<unsigned>(sqrt((pow(A, 2)) + (pow(B, 2)) + (pow(C, 2))));
}
