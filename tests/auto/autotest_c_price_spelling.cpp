#include <string>
#include <iostream>
#include "ccgi.h"
#include "cmysql.h"
#include "cuser.h"
#include "cstatistics.h"
#include "utilities.h"
#include "utilities_common.h"
#include "c_price_spelling.h"
#include "localy.h"

using namespace std;

bool Test1()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= -808;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	if(spelling == "минус восемьсот восемь рублей")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test2()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= -276498.67;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	if(spelling == "минус двести семьдесят шесть тысяч четыреста девяносто восемь рублей шестьдесят семь копеек")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test3()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= 324;
	C_Price_Spelling	price;
	auto				spelling		= price.SpellRubles(amount); // --- spell just one word

	if(spelling == "рубля")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test4()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= -0;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	if(spelling == "")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test5()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= 1000000000000000 - 1;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	if(spelling == "девятьсот девяносто девять трилионов девятьсот девяносто девять миллиардов девятьсот девяносто девять миллионов девятьсот девяносто девять тысяч девятьсот девяносто девять рублей")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test6()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= 67;
	C_Price_Spelling	price;
	auto				spelling		= price.SpellCents(amount); // --- spell just one word

	if(spelling == "копеек")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test7()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= 00019.00;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	if(spelling == "девятнадцать рублей")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test8()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= 3.1;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	if(spelling == "три рубля десять копеек")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test9()
{
	auto				result			= true;
	auto				error_message	= ""s;
	double				amount			= 26.00;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	if(spelling == "двадцать шесть рублей")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test10()
{
	auto				result			= true;
	auto				error_message	= ""s;
	double				amount			= 26.01;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	if(spelling == "двадцать шесть рублей одна копейка")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test11()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= 93.01;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	MESSAGE_DEBUG("", "", "price spellling: " + spelling);

	if(spelling == "девяносто три рубля одна копейка")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test12()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= 74.02;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	MESSAGE_DEBUG("", "", "price spellling: " + spelling);

	if(spelling == "семьдесят четыре рубля две копейки")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test13()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= 564.02;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	MESSAGE_DEBUG("", "", "price spellling: " + spelling);

	if(spelling == "пятьсот шестьдесят четыре рубля две копейки")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test14()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= 276.02;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	MESSAGE_DEBUG("", "", "price spellling: " + spelling);

	if(spelling == "двести семьдесят шесть рублей две копейки")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test15()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= 492.03;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	MESSAGE_DEBUG("", "", "price spellling: " + spelling);

	if(spelling == "четыреста девяносто два рубля три копейки")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

bool Test16()
{
	auto				result			= true;
	auto				error_message	= ""s;
	auto				amount			= 0963.04;
	C_Price_Spelling	price(amount);
	auto				spelling		= price.Spelling();

	MESSAGE_DEBUG("", "", "price spellling: " + spelling);

	if(spelling == "девятьсот шестьдесят три рубля четыре копейки")
	{

	}
	else
	{
		result = false;
		cout << "failed on " << amount << endl;
	}

	return result;
}

vector<bool (*)(void)>	func_v = {
	Test1,
	Test2,
	Test3,
	Test4,
	Test5,
	Test6,
	Test7,
	Test8,
	Test9,
	Test10,
	Test11,
	Test12,
	Test13,
	Test14,
	Test15,
	Test16,
};

bool RunTests()
{
	auto	result = true;

	for(auto f: func_v)
	{
		result &= f();
	}

	return result;
}

int main(void)
{
	CStatistics		appStat;  // --- CStatistics must be firts statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	CMysql			db;

	{ MESSAGE_DEBUG("", "", __FILE__); }
	signal(SIGSEGV, crash_handler);

	auto			testing_success = RunTests();

	if(testing_success)
		cout << "Test passed" << endl;
	else
		cout << "Test failed" << endl;


	return(0);
}

