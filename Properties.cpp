#include "Project1/HeaderFiles/Properties.h"
using namespace std;

	template<typename T, typename Property, typename NoOfParts>
	Properties<T, Property, NoOfParts>& Properties<T, Property, NoOfParts>::operator=(const Properties<T, Property, NoOfParts>& sizes)
	{
		Element_Property = sizes.Element_Property;
		Element_NoOfProperties = sizes.Element_NoOfProperties;
		Ele_TotalSizeOfParts = sizes.Ele_TotalSizeOfParts;

		return *this;
	}

	template<typename T, typename Property, typename NoOfParts>
	Properties<T, Property, NoOfParts>& Properties<T, Property, NoOfParts>::operator=(Properties&& sizes)
	{
		Element_Property = std::move(sizes.Element_Property);
		Element_NoOfProperties = std::move(sizes.Element_NoOfProperties);
		Ele_TotalSizeOfParts = std::move(sizes.Ele_TotalSizeOfParts);

		return *this;
	}

	template<typename T, typename Property, typename NoOfParts>
	template<typename U>
	void Properties<T, Property, NoOfParts>::Make_Properties(U& PairContainer)
	{
		for (auto ele : PairContainer)
		{
			const T Ref = ele.first;
			const Property size = ele.second;

			Element_Property[Ref] = size;
		}
	}

	template<typename T, typename Property, typename NoOfParts>
	template<typename U>
	void Properties<T, Property, NoOfParts>::Make_NoOfProperties(U& PairContainer)
	{
		for (auto ele : PairContainer)
		{
			const T Ref = ele.first;
			const NoOfParts noofparts = ele.second;

			Element_NoOfProperties[Ref] = noofparts;
		}
	}

	template<typename T, typename Property, typename NoOfParts>
	bool Properties<T, Property, NoOfParts>::Make_Property(const T& Ele, const Property size)
	{
		auto it = Element_Property.insert(make_pair(Ele, size));

		return it.second;
	}

	template<typename T, typename Property, typename NoOfParts>
	bool Properties<T, Property, NoOfParts>::Make_NoOfProperties(const T& Ele, const NoOfParts noofparts)
	{
		auto it = Element_NoOfProperties.insert(make_pair(Ele, noofparts));

		return it.second;
	}

	template<typename T, typename Property, typename NoOfParts>
	const Property Properties<T, Property, NoOfParts>::Get_PropertyValue(const T& Ele)const
	{
		auto it = Element_Property.find(Ele);
		if (it != Element_Property.end())
		{
			return it->second;
		}
		else
		{
			return numeric_limits<Property>::min();
		}
	}

	template<typename T, typename Property, typename NoOfParts>
	const unordered_map<T, Property>& Properties<T, Property, NoOfParts>::Get_Sizes() { return Element_Property; }

	template<typename T, typename Property, typename NoOfParts>
	const unordered_map<T, Property>& Properties<T, Property, NoOfParts>::Get_Sizes()const { return Element_Property; }

	template<typename T, typename Property, typename NoOfParts>
	Property Properties<T, Property, NoOfParts>::Get_NoOfParts(const T& Ele)
	{
		auto it = Element_NoOfProperties.find(Ele);
		if (it != Element_NoOfProperties.end())
		{
			return it->second;
		}
		else
		{
			return numeric_limits<Property>::min();
		}
	}

	template<typename T, typename Property, typename NoOfParts>
	const unordered_map<T, NoOfParts>& Properties<T, Property, NoOfParts>::Get_NoOfParts() { return Element_NoOfProperties; }