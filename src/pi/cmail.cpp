#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <memory>

#include <string>

#include "cmail.h"
#include "clog.h"
#include "localy.h"
#include "cmysql.h"
#include "cvars.h"
#include "cexception.h"
#include "ctemplate.h"

CMail::CMail() : smtpServer(SMTP_HOST), smtpPort(SMTP_PORT), smtpFlag(false)
{
	// --- this has to be Return Path in the header which must match server hostname and opendkim sign key
	mailfrom = "-f noreply@" + GetDomain();
}

unsigned long CMail::Addr2Num (const char *addr)
{
	int i;
	char tok[4];
	int octet;
	int j = 0, k = 0;
	unsigned long ipnum = 0;
	char c = 0;

	for (i=0; i<4; i++) {
		for (;;) {
			c = addr[k++];
			if (c == '.' || c == '\0') {
				tok[j] = '\0';
				octet = atoi(tok);
				if (octet > 255)
					return 0;
				ipnum += (octet << ((3-i)*8));
				j = 0;
				break;
			} else if (c >= '0' && c<= '9') {
				if (j > 2) {
					return 0;
				}
				tok[j++] = c;
			} else {
				return 0;
			}
		}
		if(c == '\0' && i<3) {
			return 0;
		}
	}
	return ipnum;
}

bool CMail::Send(string to, string subj, string mess)
{
	To(to);
	Subject(subj);
	Message(mess);
	return Send();
}

int CMail::WaitSocket(int s, int sec)
{
	fd_set		set;
	struct	timeval	tv;

	FD_ZERO(&set);
	FD_SET(s, &set);

	tv.tv_sec = RECEIVE_TIMEOUT;
	tv.tv_usec = 0;

	return(select(s + 1, &set, NULL, NULL, &tv));
}

string CMail::SocketReceive(int s)
{
	int					block_size = 1024, offset = 0;
	char				*buf;
	string				result;

	buf = (char *)malloc(sizeof(char) * block_size);
	if (buf == NULL)
	{
		CLog	log;
		log.Write(ERROR, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: error allocating buffer");

		return result;
	}
	for (offset = 0; WaitSocket(s, 3) == 1;)
	{
		int amt;

		amt = recv(s, &buf[offset], block_size - 1,0);

		if (amt == 0)
		{
			break;
		}
		else if (amt == -1)
		{
			CLog	log;
			log.Write(ERROR, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: error receiving from socket");

			free(buf);
			return result;
		}
		buf[amt] = 0;
		offset += amt;
		buf = (char *)realloc(buf, offset+block_size);
		if (buf == NULL)
		{
			CLog	log;
			log.Write(ERROR, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: error reallocating buffer");

			return result;
		}
	}
	result = buf;

	free(buf);

	return result;
}

void CMail::SocketSend(int s, string data)
{
	unsigned int	result;
	string		sendData;

	for(result = 0; result < data.length();)
	{
		sendData = data.substr(result);
		result += send(s, sendData.c_str(), sendData.length(), 0);
	}
}

bool CMail::SendSMTP()
{
	struct		sockaddr_in	sa;
	int			sock;
	string		data;
	int			isHeader;

	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": start");
	}

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		CLog	log;
		log.Write(ERROR, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: creating socket");

		return false;
	}

	memset(&sa, 0, sizeof(struct sockaddr_in));
	sa.sin_port = htons(smtpPort);
	sa.sin_addr.s_addr = inet_addr(smtpServer.c_str());
	sa.sin_family = AF_INET;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &isHeader, sizeof(isHeader));
	alarm(CONNECT_TIMEOUT);
	if(connect(sock, (struct sockaddr *)&sa, sizeof(struct sockaddr))< 0)
	{

		CLog	log;
		log.Write(ERROR, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: socket connect");

		close(sock);
		return false;
	}
	alarm(0);

	data = SocketReceive(sock);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": >", data);
	}

	data = "HELO me\n";
	SocketSend(sock, data);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": <", data);
	}

	data = SocketReceive(sock);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": >", data);
	}

	data = "MAIL FROM: <";
	data += mailfrom;
	data += ">\n";
	SocketSend(sock, data);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": <", data);
	}

	data = SocketReceive(sock);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": >", data);
	}

	if((data.find("ok") == string::npos) && (data.find("Ok") == string::npos) && (data.find("OK") == string::npos))
	{
		CLog		log;
		ostringstream	ost;

		ost << "CMail::" << __func__ << "[" << __LINE__ << "]:ERROR: sender " << mailfrom << " wrong";
		log.Write(ERROR, ost.str());
		return false;
	}

	data = "RCPT TO: <";
	data += rcptto;
	data += ">\n";
	SocketSend(sock, data);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": <", data);
	}

	data = SocketReceive(sock);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": >", data);
	}

	if((data.find("ok") == string::npos) && (data.find("Ok") == string::npos) && (data.find("OK") == string::npos))
	{
		CLog		log;
		ostringstream	ost;

		ost << "CMail::" << __func__ << "[" << __LINE__ << "]:ERROR: recipient " << rcptto << " wrong";
		log.Write(ERROR, ost.str());
		return false;
	}


	data = "DATA\n";
	SocketSend(sock, data);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": <", data);
	}

	data = SocketReceive(sock);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": >", data);
	}


	data = "subject: ";
	data += subject;
	data += "\n";
	data += message;
	data += "\n.\n";
	SocketSend(sock, data);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": <", data);
	}

	data = SocketReceive(sock);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": >", data);
	}

	data = "QUIT\n";
	SocketSend(sock, data);
	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": <", data);
	}

	close(sock);

	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": end");
	}

	return true;
}

int CMail::Input (int fd)
{
	int	result;
	/* перенаправить стандартный ввод */
	result = dup2(fd, 0);
	close(fd); 
	if(result < 0)
	    return 0;
	return 1;
}

bool CMail::isFileExist(string fname)
{
	struct stat		sb;
	bool			result = true;

	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": start");
	}

	if(stat(fname.c_str(), &sb) == -1)
	{
		result = false;
	}


	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + ": end. file is ", (!result ? "not" : ""), "exist");
	}

	return result;

}

long long CMail::getFileSize(string fname)
{
	struct stat		sb;
	long long		fileSize = 0;

	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: start");
	}

	if(stat(fname.c_str(), &sb) == -1)
	{
		fileSize = 0;
	}
	else
	{
		fileSize = sb.st_size;		
	}

	{
		CLog	log;
		log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: end (" + fname + " size is " + to_string(fileSize) + " bytes)");
	}

	return fileSize;

}

string CMail::GetTimeStamp()
{
	time_t      	now_t;
    struct tm   	*local_tm;
    ostringstream	ost;

    {
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: start";
		log.Write(DEBUG, ost.str());
    }


    now_t = time(NULL);
    local_tm = localtime(&now_t);
    if(local_tm == NULL) 
    {
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]:: ERROR in running localtime(&t)";
		log.Write(ERROR, ost.str());
    }

    ost.str("");
    ost << (local_tm->tm_year + 1900) << setw(2) << setfill('0') << (local_tm->tm_mon + 1) << setw(2) << (local_tm->tm_mday) << setw(2) << (local_tm->tm_hour) << setw(2) << (local_tm->tm_min) << setw(2) << (local_tm->tm_sec);

    {
		CLog	log;
		
		log.Write(DEBUG, "CMail::GetTimeStamp(): timestamp = ", ost.str());
    }

    {
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: end";
		log.Write(DEBUG, ost.str());
    }

    return ost.str();
}

string CMail::GetRandom(int len)
{
	string	result;
	int	i;

	for(i = 0; i < len; i++)
	{
		result += (char)('0' + (int)(rand()/(RAND_MAX + 1.0) * 10));
	}

	return result;
}


void CMail::BuildUniqPostfix()
{
	string		uniqPostfix = "";
	string		uniqFname;
	bool		isStderrUniq, isStdoutUniq, isStdinUniq;

    {
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "CMail::BuildUniqPostfix(): end";
		log.Write(DEBUG, ost.str());
    }


	do
	{
		string 			fnameStdin = MAIL_FILE_NAME;
		string 			fnameStderr = MAIL_STDERR;
		string 			fnameStdout = MAIL_STDOUT;
		struct stat		sb;


		uniqFname = "_" + GetTimeStamp() + "_" + GetRandom(10);

		fnameStdin = fnameStdin + uniqFname;
		fnameStderr = fnameStderr + uniqFname;		
		fnameStdout = fnameStdout + uniqFname;

		isStderrUniq = (stat(fnameStdin.c_str(), &sb) == -1 ? true : false);
		isStdoutUniq = (stat(fnameStderr.c_str(), &sb) == -1 ? true : false);
		isStdinUniq  = (stat(fnameStdout.c_str(), &sb) == -1 ? true : false);
	} while (!(isStdinUniq && isStdoutUniq && isStderrUniq));

	SetUniqPostfix(uniqFname);
	SetStdoutFname(MAIL_STDOUT + GetUniqPostfix());
	SetStderrFname(MAIL_STDERR + GetUniqPostfix());
	SetStdinFname(MAIL_FILE_NAME + GetUniqPostfix());

    {
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "CMail::BuildUniqPostfix(): end [" << GetUniqPostfix() << "]";
		log.Write(DEBUG, ost.str());
    }
}

// TODO: remove on July 1
/*
void CMail::RedirStdoutToFile()
{
	string		fname = GetStdoutFname();
	FILE		*fRes;

	fRes = freopen(fname.c_str(), "w", stdout);
	if(!fRes)
	{
	    {
			CLog	log;
			ostringstream	ost;

			ost.str("");
			ost << "CMail::RedirStdoutToFile: ERROR: redirect stdout to " << fname << "";
			log.Write(ERROR, ost.str());
	    }
	}
}

void CMail::RedirStderrToFile()
{
	string		fname = GetStderrFname();
	FILE		*fRes;

	fRes = freopen(fname.c_str(), "w", stderr);
	if(!fRes)
	{
	    {
			CLog	log;
			ostringstream	ost;

			ost.str("");
			ost << "CMail::RedirStderrToFile: ERROR: redirect stderr to " << fname << "";
			log.Write(ERROR, ost.str());
	    }
	}
}
*/

int CMail::SendMailExec(const char *progr, char **av, int inp)
{
	MESSAGE_DEBUG("", "", "start (" + progr + ")");

	void (*del)(int), (*quit)(int);
	int pid;

	del = signal(SIGINT, SIG_IGN); quit = signal(SIGQUIT, SIG_IGN);
	if( ! (pid = fork()))
	{   
		/* ветвление */
		/* порожденный процесс (сын) */
		signal(SIGINT, SIG_DFL); /* восстановить реакции */
		signal(SIGQUIT,SIG_DFL); /* по умолчанию         */
		{
			ostringstream	ost;
			CLog		log;

			/* getpid() выдает номер (идентификатор) данного процесса */
			pid = getpid();
			ost <<  "CMail::" << __func__ << "[" << __LINE__ << "]: child process: pid=" << pid << " run";
			log.Write(DEBUG, ost.str());
		}
		/* Перенаправить ввод-вывод */
		if(Input( inp ))
		{
			RedirStderrToFile(GetStderrFname());
			RedirStdoutToFile(GetStdoutFname());

			execvp(progr, av);
			// --- child process will stop here
			// --- execvp completely replace run-time environment to new process
		}

		// we get here just in case Input return false 
		// при неудаче печатаем причину и завершаем порожденный процесс 
		{
			CLog	log;
			log.Write(ERROR, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: redirecting input: ", strerror(errno));
		}
		/* Мы не делаем free(firstfound),firstfound = NULL
		* потому что данный процесс завершается (поэтому ВСЯ его
		* память освобождается) :
		*/
//		if( outp )  /* был создан новый файл     */
//			unlink(outp); /* но теперь он нам не нужен */
		throw CExceptionHTML("mail error");
	}
	/* процесс - отец */
	/* Сейчас сигналы игнорируются, wait не может быть оборван
	* прерыванием с клавиатуры */

	// ожидание завершения дочернего процесса
	{
		int ws;
		int pid;
		ostringstream	ost;

		while((pid = wait( &ws)) > 0 ){
			if( WIFEXITED(ws))
			{
				ost << "pid=" << pid << " died, code " << WEXITSTATUS(ws);
			}
			else if( WIFSIGNALED(ws))
			{
				ost << "pid=" << pid << " killed signal " << WTERMSIG(ws);
				if(WCOREDUMP(ws)) ost << "core dumped";
			}
			else if( WIFSTOPPED(ws))
			{
				ost << "pid=" << pid << " stopped signal " << WSTOPSIG(ws);
			}
		}
		{
			CLog	log;
			log.Write(DEBUG, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: child process: ", ost.str());
		}
	}

	/* восстановить реакции на сигналы от клавиатуры */
	signal(SIGINT, del); signal(SIGQUIT, quit);

	MESSAGE_DEBUG("", "", "finish");

	return pid;     /* вернуть идентификатор сына */
}

int CMail::MakeMailFile(string text)
{
	MESSAGE_DEBUG("", "", "start");

	int	fd, result;
	string	fname = GetStdinFname();

	fd = open(fname.c_str(), O_WRONLY | O_CREAT, 0666);
	if(fd < 0)
	{
		{
			CLog	log;
			log.Write(ERROR, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: can't create mail file");
		}
		throw CException("mail error");
	}
	result = write(fd, text.c_str(), strlen(text.c_str()));
	if(result < 0)
	{
		{
			CLog	log;
			log.Write(ERROR, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: can't write to mail file: ", strerror(errno));
		}
		throw CException("mail error");
	}
	result = fsync(fd);
	if(result < 0)
	{
		{
			CLog	log;
			log.Write(ERROR, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: can't sync data: ", strerror(errno));
		}
		throw CException("mail error");
	}
	close(fd);
	fd = open(fname.c_str(), O_RDONLY);
	if(fd < 0)
	{
		{
			CLog	log;
			log.Write(ERROR, "CMail::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: can't open mail file for read");
		}
		throw CException("mail error");
	}

	MESSAGE_DEBUG("", "", "finish");

	return fd;
}

bool CMail::SendLocal()
{
	MESSAGE_DEBUG("", "", "start");

	char	*argv[6];
	char	rcpt[64];
	char	from[64];
	long long	stderrSize, stdoutSize;

	strcpy(rcpt, rcptto.c_str());
	strcpy(from, mailfrom.c_str());
	argv[0] = const_cast<char *>("sendmail");
	argv[1] = from;
	argv[2] = rcpt;
	argv[3] = NULL;

	MESSAGE_DEBUG("", "", "params from: [" + from + "] to: [" + rcpt + "]");

	BuildUniqPostfix();
	SendMailExec(SENDMAIL_FILE_NAME, argv, MakeMailFile(message));
	
	stderrSize = getFileSize(GetStderrFname());
	stdoutSize = getFileSize(GetStdoutFname());

	MESSAGE_DEBUG("", "", "stdout size: " + to_string(stdoutSize) + ", stderr size: " + to_string(stderrSize) + "");

	if(!stdoutSize) unlink((MAIL_STDOUT + GetUniqPostfix()).c_str() );	
	if(!stderrSize) unlink((MAIL_STDERR + GetUniqPostfix()).c_str() );
	if(!(stdoutSize || stderrSize)) unlink((MAIL_FILE_NAME + GetUniqPostfix()).c_str() );

	MESSAGE_DEBUG("", "", "finish");

	return true;
}

void CMail::SetSMTP()
{
	smtpFlag = true;
}

void CMail::SetLocal()
{
	smtpFlag = false;
}

bool CMail::Send()
{
	return (smtpFlag ? SendSMTP() : SendLocal());
}








string CMailLocal::GetUserLng()
{
	ostringstream	ost;

	MESSAGE_DEBUG("", "", "start");

	ost.str("");

	if(GetUserLogin().empty() && GetUserID().empty())
		throw CExceptionHTML("recipient before template");

	if(!GetUserLogin().empty()) ost << "select lng from users where login=\"" << GetUserLogin() << "\"";
	if(!GetUserID().empty()) ost << "select lng from usere where id=\"" << GetUserID() << "\"";
	if(db->Query(ost.str()) == 0) throw CExceptionHTML("no user");

	MESSAGE_DEBUG("", "", "finish");

	return db->Get(0, "lng");
}

string CMailLocal::SetTemplate(string templID)
{
	return SetTemplateFile(TEMPLATE_PATH + GetUserLng() + "/mails/" + templID + ".htmlt");
}

string CMailLocal::SetTemplateFile(string fileName)
{
    FILE	*templateFile;
    templateFile = fopen(fileName.c_str(), "r");
    if(!templateFile)
    {
		CLog log;
		log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: can't open file", fileName);
		templateFile = NULL;

		throw CExceptionHTML("mail template");
    }
    fclose(templateFile);

    return fileName;
}

void CMailLocal::Send(string login, string templID, CVars *v, CMysql *db)
{
	string			email;

	MESSAGE_DEBUG("", "", "start (" + login + ", " + templID + ")");

	if(db->Query("select `email` from `users` where `login`=\"" + login + "\";") == 0)
	{
		MESSAGE_ERROR("", "", "select email from table return empty result");

		throw CExceptionHTML("user mail failed");
	}

	if((email = db->Get(0, "email")).empty())
	{
		MESSAGE_ERROR("", "", "mail length is zero");

		throw CExceptionHTML("user mail failed");
	}
	else
	{
		v->Redefine("RECIPIENT_EMAIL", email);
	}

	SendToEmail(email, login, templID, v, db);

	MESSAGE_DEBUG("", "", "finish");
}

void CMailLocal::SendToEmail(string email, string login, string templID, CVars *v, CMysql *mysql)
{
	MESSAGE_DEBUG("", "", "start (" + email + ", " + login + ", " + templID + ")");

	auto				message = ""s;
	auto				trySubj = ""s;
	CTemplate			templ;
	string::size_type	p1;

	SetDB(mysql);
	UserLogin(login);
	SetVars(v);

	templ.SetFile(SetTemplate(templID));
	templ.SetVars(vars);
	message = templ.Render();

	if(message.empty())
	{
		MESSAGE_ERROR("", "", "message length is zero");

		throw CExceptionHTML("user mail failed");
	}

	p1 = message.find("subject:");
	if(p1 != string::npos)
	{
		trySubj = message.substr(p1 + 8, message.find("\n") - p1 - 8);
		message.erase(0, message.find("\n") + 1);
	}

	if(!CMail::Send(email, trySubj, message))
	{
		throw CExceptionHTML("mail error");
	}

	MESSAGE_DEBUG("", "", "finish");
}

