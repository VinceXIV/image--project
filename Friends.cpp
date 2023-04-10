#include "HeaderFiles/GeneralFunctions.h"
#include "HeaderFiles/Friends.h"

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

template<typename T, typename Property, typename Number>
bool Friends<T, Property, Number>::operator==(const Friends& rhs) { return friends == rhs.friends; }

template<typename T, typename Property, typename Number>
bool Friends<T, Property, Number>::operator!=(const Friends& rhs) { return !(operator==(rhs)); }

template<typename T, typename Property, typename Number>
void Friends<T, Property, Number>::clear() { friends.clear(); }

template<typename T, typename Property, typename Number>
bool Friends<T, Property, Number>::empty() { return friends.empty(); }

template<typename T, typename Property, typename Number>
size_t Friends<T, Property, Number>::size() { return friends.size(); }

template<typename T, typename Property, typename Number>
size_t Friends<T, Property, Number>::size()const { return friends.size(); }

template<typename T, typename Property, typename Number>
unordered_map<T, unordered_map<T, unsigned>> Friends<T, Property, Number>::Compare_Sizes(const Friends& AnotherFriends)const
{
	return Combine_Distances(Compare_Keys(AnotherFriends), Compare_Friends(AnotherFriends));
}

template<typename T, typename Property, typename Number>
template<typename U>
void Friends<T, Property, Number>::Make_friends(const T& Key, U& Container)
{
	for (auto Ele : Container)
	{
		if (Ele != Key)
		{
			friends[Key].insert(Ele);
		}
	}
}

template<typename T, typename Property, typename Number>
template<typename U>
void Friends<T, Property, Number>::Make_MutualFriends(const U& Container)
{
	if (Container.size() == 1)
	{
		Add_friend(*(Container.begin()));
	}

	for (auto Key : Container)
	{
		for (auto Ele : Container)
		{
			if (Ele != Key)
			{
				friends[Key].insert(Ele);
			}
		}
	}
}

template<typename T, typename Property, typename Number>
void Friends<T, Property, Number>::Add_friend(const T& Key, const T& P)
{
	if (Key != P)
	{
		friends[Key].insert(P);
	}
}

template<typename T, typename Property, typename Number>
bool Friends<T, Property, Number>::AreMutualFriends(const T& EleX, const T& EleY)
{
	return (IsAFriend(EleX, EleY) && IsAFriend(EleY, EleX));
}

template<typename T, typename Property, typename Number>
bool Friends<T, Property, Number>::IsAFriend(const T& Ref, const T& Ele)
{
	const unordered_set<T>& FriendsOfRef = friends.find(Ref)->second;

	return FriendsOfRef.end() != FriendsOfRef.find(Ele);
}

template<typename T, typename Property, typename Number>
void Friends<T, Property, Number>::Add_friend(const T& Key)
{
	friends[Key];
}

template<typename T, typename Property, typename Number>
void Friends<T, Property, Number>::Add_Friends(const T& Key, const T& friend1, const T& friend2)
{
	Add_friend(Key, friend1);
	Add_friend(Key, friend2);
}

template<typename T, typename Property, typename Number>
void Friends<T, Property, Number>::Unfriend(const T& Key, const T& P)
{
	friends[Key].erase(P);
}

template<typename T, typename Property, typename Number>
vector<T> Friends<T, Property, Number>::Get_friends(const T& Key)
{
	vector<T> Ret;

	auto it = friends.find(Key);

	if (it != friends.end())
	{
		unordered_set<T> F = it->second;
		Ret.reserve(F.size());

		copy(F.begin(), F.end(), back_inserter(Ret));
	}

	return Ret;
}

template<typename T, typename Property, typename Number>
const vector<T>& Friends<T, Property, Number>::Get_friends(const T& Key)const
{
	vector<T> Ret;

	auto it = friends.find(Key);

	if (it != friends.end())
	{
		unordered_set<T> F = it->second;
		Ret.reserve(F.size());

		copy(F.begin(), F.end(), back_inserter(Ret));
	}

	return Ret;
}

template<typename T, typename Property, typename Number>
void Friends<T, Property, Number>::Get_friends(vector<T>& Container)const
{
	Container.reserve(friends.size());
	for (auto ele : friends)
	{
		Container.push_back(ele.first);
	}
}

template<typename T, typename Property, typename Number>
void Friends<T, Property, Number>::Get_friends(unordered_set<T>& Container)const
{
	Container.reserve(friends.size());

	for (auto ele : friends)
	{
		Container.insert(ele.first);
	}
}

template<typename T, typename Property, typename Number>
unsigned Friends<T, Property, Number>::Get_NoOfFriends(const T& Key)
{
	auto it = friends.find(Key);

	if (it != friends.end())
	{
		return it->second;
	}

	return 0;
}

template<typename T, typename Property, typename Number>
const unordered_map<T, unordered_set<T>>& Friends<T, Property, Number>::Get()const { return friends; }

template<typename T, typename Property, typename Number>
unordered_map<T, unordered_set<T>>& Friends<T, Property, Number>::Get() { return friends; }

template<typename T, typename Property, typename Number>
unordered_map<T, unordered_map<T, unsigned>> Friends<T, Property, Number>::Compare_Keys(const Friends& AnotherFriends)const
{
	unordered_map<T, unordered_map<T, unsigned>> Ref_Ele_Difference;

	const unordered_map<T, unordered_set<T>> anotherfriends = AnotherFriends.Get();

	for (auto refkey : friends)
	{
		const Property refsize = this->Get_PropertyValue(refkey.first);

		for (auto key : anotherfriends)
		{
			const Property size = AnotherFriends.Get_PropertyValue(key.first);

			unsigned Difference = Get_Difference(refsize, size);

			Ref_Ele_Difference[refkey.first][key.first] = Difference;
		}
	}

	return Ref_Ele_Difference;
}

template<typename T, typename Property, typename Number>
unordered_map<T, unordered_map<T, unsigned>> Friends<T, Property, Number>::Compare_Friends(const Friends<T, Property, Number>& AnotherFriends)const
{
	unordered_map<T, unordered_map<T, unsigned>> Ref_Ele_SizeDifference;

	const unordered_map<T, unordered_set<T>>& anotherfriends = AnotherFriends.Get();

	for (auto refkey : friends)
	{
		const size_t NoOfFriendsOfRef = refkey.second.size();
		for (auto key : anotherfriends)
		{
			const size_t NoOfFriendsOfEle = key.second.size();
			multimap<unsigned, pair<T, T>> SizeDifferences_Ref_Ele;
			for (auto RefFriend : refkey.second)
			{
				const Property RefFriendSize = this->Get_PropertyValue(RefFriend);

				for (auto AnotherFriend : key.second)
				{
					const Property AnotherFriendSize = AnotherFriends.Get_PropertyValue(AnotherFriend);

					unsigned SizeDifference = Get_Difference(RefFriendSize, AnotherFriendSize);
					const pair<T, T> Ref_Ele = make_pair(RefFriend, AnotherFriend);
					SizeDifferences_Ref_Ele.insert(std::move(make_pair(SizeDifference, Ref_Ele)));
				}
			}


			unordered_map<T, bool> RefIsUsed;
			unordered_map<T, bool> EleIsUsed;
			auto Diff = SizeDifferences_Ref_Ele.begin();
			unsigned FriendDifference = 0;
			for (size_t i = 0, j = 0; i < NoOfFriendsOfRef || j < NoOfFriendsOfEle; ++i, ++j)
			{
				const T& Ref = Diff->second.first;
				const T& Ele = Diff->second.second;
				if (i < NoOfFriendsOfRef && j < NoOfFriendsOfEle)
				{
					//first pick the best combination
					if (!RefIsUsed[Ref] && !EleIsUsed[Ele])
					{
						FriendDifference += Diff->first;

						RefIsUsed[Ref] = true;
						EleIsUsed[Ele] = true;
						++Diff;
					}
				}
				//then others
				else if (i >= NoOfFriendsOfRef && j < NoOfFriendsOfEle)
				{
					if (!EleIsUsed[Ele])
					{
						FriendDifference += Diff->first;

						EleIsUsed[Ele] = true;
						++Diff;
					}
				}
				else if (i < NoOfFriendsOfRef && j >= NoOfFriendsOfEle)
				{
					if (!RefIsUsed[Ref])
					{
						FriendDifference += Diff->first;

						RefIsUsed[Ref] = true;
						++Diff;
					}
				}
			}

			Ref_Ele_SizeDifference[refkey.first][key.first] = FriendDifference;
		}
	}

	return Ref_Ele_SizeDifference;
}

template<typename T, typename Property, typename Number>
unordered_map<T, unordered_map<T, unsigned>>
	Friends<T, Property, Number>::Combine_Distances(unordered_map<T, unordered_map<T, unsigned>>&& Ref, unordered_map<T, unordered_map<T, unsigned>>&& Ele)const
{
	for (auto refkey : Ref)
	{
		for (auto EleBeingcompared : refkey.second)
		{
			Ref[refkey.first][EleBeingcompared.first] += Ele[refkey.first][EleBeingcompared.first];
		}
	}

	return Ref;
}
