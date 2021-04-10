#ifndef __C_CONFIG__H__
#define __C_CONFIG__H__

#include <iostream>
#include <fstream>
#include <regex>
#include <tuple>
#include <vector>
#include <map>

#include "clog.h"

using namespace std;

class c_config_cache
{
	private:
		const string			DEFAULT_KEY	= "__default__";
		map<string, map<string, string>>	content;

	public:
		void					Add(const string &file, const map<string, string> &file_content) { content[file] = file_content; };
		bool					IsInCache(const string &file);
		bool					IsInCache(const string &file, const vector<string> &entry);
		map<string, string>		Get(const string &file, const vector<string> &entries);
};

class c_config_tag
{
	private:

		enum {VAR, CONFIG}		tag_type;
		string					VAR_EXPR		= "var:";
		string					CONFIG_EXPR		= "config:";
		string					TAG_SEPARATOR	= ":";

		string					line			= "";

		string::size_type		start_pos		= string::npos;
		string::size_type		end_pos			= string::npos;

		string					file			= "";
		string					var				= "";

	public:

								c_config_tag(const string &param) : line{param} {}; 

		void					UpdateData(const string &param) { line = param; };

		bool					isFound();
		bool					isTypeVar()	const	{ return tag_type == VAR; };
		bool					isTypeConfig() const{ return tag_type == CONFIG; };

		string::size_type		GetStartPos() const	{ return start_pos; };
		string::size_type		GetEndPos()	const	{ return end_pos; };

		string					GetFile() const		{ return file; };
		string					GetVar() const		{ return var; };
};

class c_config
{
	private:
		string					config_folder				=	""s;

		c_config_cache			cache;

		// --- multiline related definitions
		enum state_enum 		{ KEY_VALUE_PAIR, MULTILINE_VALUE };
		const string			MULTILINE_MARKER = "\'\'\'";
		bool					IsMultilineMarkerPresent(const string &line);
		bool		 			IsStartFromMultilineMarker(const string &line);
		bool		 			IsLineEndsWithMultilineMarker(const string &line);
		string					StripMultilineMarker(string line);

		// --- support class methods
		tuple<string, string>	ExtractKeyValue(const string &line);
		string					RemoveComment(string line);
		map<string, string>		Render(map<string, string> result, const map<string, string> &vars);
		map<string, string>		ReadFileContent(const string &file);
		string					trim(string line);

		bool					isVarChecksValid(const c_config_tag &tag, const map<string, string> &vars);
		bool					isConfigChecksValid(const c_config_tag &tag);
		string 					RenderVarInLine(string line, const c_config_tag &tag, const string &var_value);

	public:
								c_config(const string &folder) : config_folder {folder} {};

		map<string, string>		GetFromFile(const string &file, const vector<string> &keys, const map<string, string> &vars);
		map<string, string>		GetFromFile(const string &file, const vector<string> &keys);
		string					GetFromFile(const string &file, const string &keys);

		void					SetConfigFolder(const string &param)					{ config_folder = param; }
		string					GetConfigFolder()										{ return config_folder; }

};

ostream&	operator<<(ostream& os, const c_config &);

#endif
