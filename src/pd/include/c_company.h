#ifndef __C_COMPANY__H__
#define __C_COMPANY__H__

#include <string>

#include "cuser.h"
#include "mysql.h"
#include "clog.h"
#include "utilities_infomed1.h"

using namespace std;

class C_Company
{
	private:
		CMysql		*db = NULL;
		CUser		*user = NULL;

		string		id					= "";
		string		name				= "";
		string		type				= "";
		string		legal_geo_zip_id	= "";
		string		legal_geo_zip		= "";
		string		legal_address		= "";
		string		mailing_geo_zip_id	= "";
		string		mailing_geo_zip		= "";
		string		mailing_address		= "";
		string		tin					= "";
		string		vat					= "";
		string		bik					= "";
		string		bank_id				= "";
		string		account				= "";
		string		kpp					= "";
		string		ogrn				= "";
		string		link				= "";
		string		admin_userID		= "";
		string		webSite				= "";
		string		description			= "";
		string		isBlocked			= "";
		string		logo_folder			= "";
		string		logo_filename		= "";

	public:
					C_Company();
					C_Company(CMysql *, CUser *);

		void		SetID					(const string &param)	{ id = param; };
		void		SetID					(string &&param)		{ id = move(param); };
		void		SetName					(const string &param)	{ name = param; };
		void		SetName					(string &&param)		{ name = move(param); };
		void		SetType					(const string &param)	{ type = param; };
		void		SetType					(string &&param)		{ type = move(param); };
		void		SetLegalGeoZipID		(const string &param)	{ legal_geo_zip_id = param; };
		void		SetLegalGeoZipID		(string &&param)		{ legal_geo_zip_id = move(param); };
		void		SetLegalGeoZip			(const string &param)	{ legal_geo_zip = param; };
		void		SetLegalGeoZip			(string &&param)		{ legal_geo_zip = move(param); };
		void		SetLegal_address		(const string &param)	{ legal_address = param; };
		void		SetLegal_address		(string &&param)		{ legal_address = move(param); };
		void		SetMailingGeoZipID		(const string &param)	{ mailing_geo_zip_id = param; };
		void		SetMailingGeoZipID		(string &&param)		{ mailing_geo_zip_id = move(param); };
		void		SetMailingGeoZip		(const string &param)	{ mailing_geo_zip = param; };
		void		SetMailingGeoZip		(string &&param)		{ mailing_geo_zip = move(param); };
		void		SetMailing_address		(const string &param)	{ mailing_address = param; };
		void		SetMailing_address		(string &&param)		{ mailing_address = move(param); };
		void		SetTIN					(const string &param)	{ tin = param; };
		void		SetTIN					(string &&param)		{ tin = move(param); };
		void		SetVAT					(const string &param)	{ vat = param; };
		void		SetVAT					(string &&param)		{ vat = move(param); };
		void		SetBIK					(const string &param)	{ bik = param; };
		void		SetBIK					(string &&param)		{ bik = move(param); };
		void		SetBankID				(const string &param)	{ bank_id = param; };
		void		SetBankID				(string &&param)		{ bank_id = move(param); };
		void		SetAccount				(const string &param)	{ account = param; };
		void		SetAccount				(string &&param)		{ account = move(param); };
		void		SetKPP					(const string &param)	{ kpp = param; };
		void		SetKPP					(string &&param)		{ kpp = move(param); };
		void		SetOGRN					(const string &param)	{ ogrn = param; };
		void		SetOGRN					(string &&param)		{ ogrn = move(param); };
		void		SetLink					(const string &param)	{ link = param; };
		void		SetLink					(string &&param)		{ link = move(param); };
		void		SetAdmin_userID			(const string &param)	{ admin_userID = param; };
		void		SetAdmin_userID			(string &&param)		{ admin_userID = move(param); };
		void		SetWebSite				(const string &param)	{ webSite = param; };
		void		SetWebSite				(string &&param)		{ webSite = move(param); };
		void		SetDescription			(const string &param)	{ description = param; };
		void		SetDescription			(string &&param)		{ description = move(param); };
		void		SetIsBlocked			(const string &param)	{ isBlocked = param; };
		void		SetIsBlocked			(string &&param)		{ isBlocked = move(param); };
		void		SetLogo_folder			(const string &param)	{ logo_folder = param; };
		void		SetLogo_folder			(string &&param)		{ logo_folder = move(param); };
		void		SetLogo_filename		(const string &param)	{ logo_filename = param; };
		void		SetLogo_filename		(string &&param)		{ logo_filename = move(param); };

		auto		GetID					()						{ return id; };
		auto		GetName					()						{ return name; };
		auto		GetType					()						{ return type; };
		auto		GetLegalGeoZipID		()						{ return legal_geo_zip_id; };
		auto		GetLegalGeoZip			()						{ return legal_geo_zip; };
		auto		GetLegal_address		()						{ return legal_address; };
		auto		GetMailingGeoZipID		()						{ return mailing_geo_zip_id; };
		auto		GetMailingGeoZip		()						{ return mailing_geo_zip; };
		auto		GetMailing_address		()						{ return mailing_address; };
		auto		GetTIN					()						{ return tin; };
		auto		GetVAT					()						{ return vat; };
		auto		GetBIK					()						{ return bik; };
		auto		GetBankID				()						{ return bank_id; };
		auto		GetAccount				()						{ return account; };
		auto		GetKPP					()						{ return kpp; };
		auto		GetOGRN					()						{ return ogrn; };
		auto		GetLink					()						{ return link; };
		auto		GetAdmin_userID			()						{ return admin_userID; };
		auto		GetWebSite				()						{ return webSite; };
		auto		GetDescription			()						{ return description; };
		auto		GetIsBlocked			()						{ return isBlocked; };
		auto		GetLogo_folder			()						{ return logo_folder; };
		auto		GetLogo_filename		()						{ return logo_filename; };

		auto		GetByID					(const string param)	{ return GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE  `id`=\"" + param + "\";", db, user);};
		auto		GetByTIN				(const string param)	{ return GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE `tin`=\"" + param + "\";", db, user);};

		auto		CheckValidity			() -> string;
		auto		InsertToDB				() -> string;
};

ostream&	operator<<(ostream& os, const C_Company &);

#endif
