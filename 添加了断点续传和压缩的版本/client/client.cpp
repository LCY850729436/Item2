#define _CRT_SECURE_NO_WARNINGS

#include"Client.h"
#include"unzip.h"
#include"zip.h"

#define ZIPSIZE 100000

SOCKET Client::s_server;
//SOCKET Client::s_accept;
//����˵�ַ�ͻ��˵�ַ
SOCKADDR_IN Client::server_addr;
//SOCKADDR_IN Client::accept_addr;
string Client::file_path;
int Client::mother_file_index_from_start = 0;
int Client::mother_file_length = 0;
int Client::p_counter = 0;

string server_ip;
int port;

ofstream fout;
ofstream fout1;
ofstream fout2;
ofstream fout3;
ifstream fin;
ifstream fin1;

void Client::initialization()
{
	//��ʼ���׽��ֿ�
	WORD w_req = MAKEWORD(2, 2);//�汾��	
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0)
	{
		cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << endl;
	}
	else
	{
		cout << "��ʼ���׽��ֿ�ɹ���" << endl;
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
	{
		cout << "�׽��ֿ�汾�Ų�����" << endl;		WSACleanup();
	}
	else
	{
		cout << "�׽��ֿ�汾��ȷ��" << endl;
	}
	//������˵�ַ��Ϣ
}

bool Client::file_transmision(string file_name)
{
	int recv_len;
	char recv_buf[100];
	int readlen;
	//char buf[1025];
	char buf[HEAD_SIZE+BUFFER_SIZE] = { 0 };
	int send_len;
	ifstream in_file(file_name, ios::in | ios::binary);
	//in_file >> noskipws;
	if (!in_file.is_open())
	{
		cout << "�Ҳ����ļ�" << file_name << endl;
		in_file.close();
		return false;
	}
	while (!in_file.eof())
	{
		//cout << 1;
		buf[0] = '0'; buf[1] = '0'; buf[2] = '1';
		char* infor_p = buf + HEAD_SIZE;
		in_file.read(infor_p, BUFFER_SIZE);
		//cout << buf << endl;
		readlen = in_file.gcount();
		if (readlen == 1024)
		{
			buf[3] = '1'; buf[4] = '0'; buf[5] = '2'; buf[6] = '4';
		}
		else
		{
			set_file_length(buf, readlen);
		}
		
		
		send_len = send(s_server, buf, readlen + HEAD_SIZE, 0);
		if (send_len < 0)
		{
			cout << "����ʧ��" << endl;
			in_file.close();
			return false;
		}
		/*p_counter++;
		if (p_counter >= 10240)
		{
			p_counter = 0;
			Sleep(200);
		}*/
	}
	//cout << "�ļ� " << file_name << " �������"<<endl;
	in_file.close();
	//buf[0] = '0'; buf[1] = '0'; buf[2] = '2';
	//Sleep(1);
	//Sleep(0.5);
	send_len = send(s_server, "0020021file_transmission_end", 21+HEAD_SIZE, 0);//25
	if (send_len < 0)
	{
		cout << "�����ļ��������ָ��ʧ��" << endl;
		return false;
	}
	cout << endl;
	return true;
}

bool Client::sent_create_file_messege(string file_path)
{
	file_path = file_path.substr(mother_file_index_from_start);

	int send_len;
	
	string file_len = to_string(file_path.length());
	while (file_len.length() < 4)
	{
		file_len = "0" + file_len;
	}
	string sent_str = "003" + file_len + file_path;
	if (send_len = send(s_server, sent_str.c_str(), sent_str.length(), 0))
	{
		cout << "���ʹ����ļ��гɹ�" << file_path << endl;
	}
	else
	{
		cout << "���ʹ����ļ���ָ��ʧ��" << file_path << endl;
		return false;
	}
	return true;
}

bool Client::sent_file_name(string file_name)
{
	file_name = file_name.substr(mother_file_index_from_start);

	int send_len;
	string file_len = to_string(file_name.length());
	while (file_len.length() < 4)
	{
		file_len = "0" + file_len;
	}
	string sent_str = "004" + file_len + file_name;
	if (send_len = send(s_server, sent_str.c_str(), sent_str.length(), 0))//200
	{
		cout << "���ʹ����ļ��ɹ�" << file_name << endl;
	}
	else
	{
		cout << "���ʹ����ļ���ָ��ʧ��" << file_name << endl;
		return false;
	}
	return true;
}

bool Client::sent_end_transmission()
{
	int send_len;
	string temp = "all_transmission_end";
	if (send_len = send(s_server, ("0000020" + temp).c_str(), ("0000020" + temp).length(), 0))
	{
		cout << "�Ѿ������������ָ��" << endl;
	}
	else
	{
		cout << "�������ָ�������" << endl;
		return false;
	}
	return true;
}

void Client::getAllFiles(string path, vector<string>& files, vector<int>& is_file_packet, int hierarchy) {
	//�ļ����
	long hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;  //�����õ��ļ���Ϣ��ȡ�ṹ
	string p;  //string�������˼��һ����ֵ����:assign()���кܶ����ذ汾

	if (hierarchy == 0)
	{
		if (_findfirst(p.assign(path).c_str(), &fileinfo) != -1)
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				cout << "Դ�ļ����ļ���" << endl;
				files.push_back(p.assign(path));
				is_file_packet.push_back(1);
			}
			else
			{
				cout << "Դ�ļ�����һ���ļ�" << endl;
				files.push_back(p.assign(path));
				is_file_packet.push_back(0);
			}
		}
	}

	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1) {
		do {
			//cout << fileinfo.name << endl;
			if ((fileinfo.attrib & _A_SUBDIR)) {  //�Ƚ��ļ������Ƿ����ļ���
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					files.push_back(p.assign(path).append("/").append(fileinfo.name));
					is_file_packet.push_back(1);
					getAllFiles(p.assign(path).append("/").append(fileinfo.name), files, is_file_packet, hierarchy + 1);

				}

			}
			else {
				files.push_back(p.assign(path).append("/").append(fileinfo.name));
				is_file_packet.push_back(0);
			}

		} while (_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
		_findclose(hFile);

	}

}

void Client::__connect()
{
	int recv_len;
	char recv_buf[100];
	initialization();
	//���������Ϣ

	fin.open("record.txt");
	int branch = 0;//�����жϳ����ϴδ����Ƿ��ж�,��Ϊ0����û�жϣ�Ϊ1�ж�
	if (!fin.is_open())branch = 0;//�����ļ���ʧ�ܣ���˵��Ϊ��һ��ʹ�øó���
	else
	{
		string str;
		getline(fin, str);
		if (str == "Finished")branch = 0;
		else branch = 1;
		fin.close();
	}

	//�������û���жϣ��½�������дrecord�ļ�
	if (branch == 0)
	{
		fout.open("record.txt");
		fout << "Finished";
		fout.close();
	}

	cout << "�����롾�ļ���ַ�� ��ip�� ���˿ںš��м���һ���ո����" << endl;
	cin >> file_path >> server_ip >> port;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip.c_str()); //192.168.20.1     127.0.0.1
	server_addr.sin_port = htons(port);
	//�����׽���
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	while (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "����������ʧ�ܣ����ڳ���������" << endl;
		WSACleanup();
		Sleep(2000);//�ȴ�2�����������
		initialization();
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip.c_str()); //192.168.20.1     127.0.0.1
		server_addr.sin_port = htons(port);
		//�����׽���
		s_server = socket(AF_INET, SOCK_STREAM, 0);
	}
	
	cout << "���������ӳɹ���" << endl;
	
	if (recv_len = recv(s_server, recv_buf, 100, 0) < 0)
	{
		cout << "������,û�������յ��ظ�" << endl;
	}

	//����,��������


	/*if (file_path[file_path.length() - 1] != '/' && file_path[file_path.length() - 1] != '\\')
	{
		file_path += '/';
	}*/
	if (_access(file_path.c_str(), 0) == -1)	//����ļ��в�����
	{
		cout << "�����ڸ��ļ��л����ļ�" << endl;
		exit(-1);
	}
}

//��������з������Ͽ���������
void Client::re__connect()
{
	int recv_len;
	char recv_buf[100];

	cout << "����˺�������ܶϿ������ڳ�������" << endl;
	WSACleanup();
	Sleep(2000);
	initialization();
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip.c_str()); //192.168.20.1     127.0.0.1
	server_addr.sin_port = htons(port);
	//�����׽���
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	while (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "����������ʧ�ܣ����ڳ���������" << endl;
		WSACleanup();
		Sleep(2000);
		initialization();
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip.c_str()); //192.168.20.1     127.0.0.1
		server_addr.sin_port = htons(port);
		//�����׽���
		s_server = socket(AF_INET, SOCK_STREAM, 0);
	}

	cout << "�������������ӳɹ���" << endl;

	if (recv_len = recv(s_server, recv_buf, 100, 0) < 0)
	{
		cout << "������,û�������յ��ظ�" << endl;
	}
}

//����ļ����һ�ε��޸�ʱ��
string GetFileTime(string filepath)
{
	struct stat buf;
	stat(filepath.c_str(), &buf);
	time_t time1 = buf.st_mtime;
	tm *ptm = localtime(&time1);

	char str[100];
	sprintf(str, "%d/%02d/%02d %02d:%02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1,
		ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	string modify_time = str;
	return modify_time;
}


void Client::__trans(int &flag)
{
	/*
	���ļ���¼������������ϴδ�����������������ΪFinished����һ��
	��û���������䣬��һ��ΪUnfinished���ڶ���Ϊ�ϴδ������ʱ���ڴ�����ļ�����ţ�������
	*/
	string FilePath = "record.txt";
	const char* file = FilePath.c_str();
	FILE * fr;
	char path[100];
	sprintf_s(path, 100, file, (1));
	fopen_s(&fr, path, "rb");
	char  temp[1];
	vector<char>file_content;
	int count = 1;
	if (fr != NULL)
	{
		while (!feof(fr)) {
			temp[0] = fgetc(fr);
			file_content.push_back(temp[0]);
		}
		fclose(fr);
		file_content.pop_back();

		for (int i = 0;i < file_content.size();i++)
		{
			if (file_content[i] == '\n')count++;//ͳ�ƻ��з��������ж�����
		}
	}

	clock_t start_t, ends_t;
	vector<string> files;
	vector<int> is_file_packet;
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	char recv_buf[100];
	int file_name_head = 0, file_name_end = file_path.length() - 1;
	string mother_file;
	int mother_file_length;

	start_t = clock();
	for (int j = file_path.length() - 1; j >= 0; j--)
	{
		if (file_path[j] != '\\' && file_path[j] != '/')
		{
			file_name_end = j;
			break;
		}
	}
	for (int j = file_name_end; j >= 0; j--)
	{
		if (file_path[j] == '\\' || file_path[j] == '/')
		{
			file_name_head = j + 1;
			break;
		}
	}
	mother_file = file_path.substr(file_name_head, file_name_end - file_name_head + 1);//��ȡmother_file,����������Ǿ���·��,Ҳ������ȷ�������·��
	cout << "mother file is " << mother_file << endl;
	mother_file_index_from_start = file_name_head;
	mother_file_length = file_name_end - file_name_head + 1;


	getAllFiles(file_path, files, is_file_packet, 0);

	int diff = 0;//��¼�ļ��Ƿ���ֲ��죬��Ϊ0�������û�в��죬�����в��죬��Ҫȫ���ش�
	fin.open("sended.txt");//���ļ���¼�Ѿ��ϴδ���ʱ�Ѿ�����õ��ļ����ļ���
	fin1.open("time.txt");//���ļ���¼�Ѵ����ļ�������޸�ʱ��
	vector<string>sendedPath;
	vector<string>times;
	if (count != 1)
	{
		if (!fin.is_open() || !fin1.is_open())diff = 0;//�����ļ���ʧ�ܣ���˵��Ϊ��һ��ʹ�øó���
		else
		{
			while (!fin.eof())
			{
				string s;
				getline(fin, s);
				sendedPath.push_back(s);
			}
			sendedPath.pop_back();
			while (!fin1.eof())
			{
				string s;
				getline(fin1, s);
				times.push_back(s);
			}
			times.pop_back();

			int struct_diff = 0;//����ļ��ṹ��ͬ����Ϊ0������ͬΪ-1
			//��ʼ�Ƚ��Ѵ�����ļ�Ŀ¼�ṹ
			for (int i = 0;i < sendedPath.size() && i < files.size();i++)
			{
				if (sendedPath[i] != files[i])
				{
					diff = -1;
					struct_diff = -1;
				}
				//���������Ҫ������ļ��ṹ�����ļ����ݷ����˱仯������Ҫ�ش�
			}

			//���ļ�Ŀ¼�ṹû�䣬��Ƚ��ļ�������޸�ʱ�����ж��ļ��������Ƿ����˱仯
			if (struct_diff == 0)
			{
				//���Ѵ�����ļ��к��ļ���ѡ���ļ�
				vector<string>sendedfiles;
				for (int i = 0;i < sendedPath.size();i++)
					if (!is_file_packet[i])sendedfiles.push_back(sendedPath[i]);
				for (int i = 0;i < sendedfiles.size();i++)
				{
					string modify_time = GetFileTime(sendedfiles[i]);
					if (modify_time != times[i])diff = -1;
				}
			}
		}
	}
	fin.close();
	fin1.close();
	//�ϴδ���������������Ҫ�ش������sended�ļ���time�ļ�
	if (count == 1 || diff == -1)
	{
		fout1.open("sended.txt");
		fout3.open("time.txt");
	}
	else
	{
		fout1.open("sended.txt", ios::app);
		fout3.open("time.txt", ios::app);
	}
	fout1.close();
	fout3.close();

	cout << files.size() << " " << is_file_packet.size() << endl;

	int num = 0;//����ʱ����һ���ļ���ʼ��

	//�ϴδ��䱻�жϵ����
	if (count != 1)
	{
		//�����⵽�ļ��ṹ�����仯�������˷���ɾ���Ѿ�����õ��ļ���ָ��ٽ����ش�
		if (diff == -1)
		{
			cout << endl << "��⵽�ϴδ����жϣ����Ѵ��䲿�ַ����䶯����ɾ���Ѿ�������ļ������ش������ļ�" << endl;

			//�����˷���ɾ���Ѿ�ɾ�����ļ�ָ��
			int send_len;
			string file_len = "0000";
			if (send_len = send(s_server, ("111"+file_len).c_str(), ("111" + file_len).length(), 0))
			{
				cout << "����ɾ���ļ���ָ��ɹ�" << endl;
			}
			else
			{
				cout << "����ɾ���ļ���ָ��ʧ��" << endl;
				re__connect();
				return;
			}

			cout << endl << "��ɾ���ϴ��Ѿ�������ļ�" << endl;
			cout << endl << "��ʼ�ش�" << endl << endl;
		}
		else
		{
			//�ļ��ṹû�з����仯������ϴ��жϴ���ʼ����
			ifstream fin("record.txt");
			string instruct;
			getline(fin, instruct);
			getline(fin, instruct);
			stringstream ss(instruct);
			ss >> num;
			fin.close();
			cout << "��⵽�ϴδ��䱻�жϣ����Ѵ��䲿��û�б䶯������" << files[num + 1] << "��ʼ����" << endl << endl;
			num++;
		}
	}

	for (int i = 0; i < files.size(); i++)
	{
		if (is_file_packet[i])
		{
			if (!sent_create_file_messege(files[i]))
			{
				re__connect();
				return;
			}
			//��¼�������
			fout.open("record.txt");
			fout << "Unfinished" << endl;
			fout << i;
			fout.close();
			
			//��¼�Ѵ�����ļ���
			fout1.open("sended.txt", ios::app);
			fout1 << files[i] << endl;
			fout1.close();
		}
		else
		{
			//���ж��ļ���С������ļ���СС��100KB���򽫸��ļ�ѹ�����䣬����ѹ��
			ifstream finsize(files[i]);
			finsize.seekg(0, ios::end);
			int size = finsize.tellg();
			finsize.close();

			
			//����ѹ��
			if (size <= ZIPSIZE && size > 0)
			{
				cout << "���ļ�С��100KB������ѹ��" << endl;
				string str1 = files[i];
				string str2 = files[i];

				int pos = 0;
				pos = files[i].rfind("/");

				int pos1 = 0;
				pos1 = files[i].rfind('.');

				str1 = str1.erase(pos + 1, files[i].length());
				str2 = str2.substr(pos + 1, pos1);

				pos1 = str2.find(".");

				string str3 = str2;

				str2 = str2.substr(0, pos1);

				string zippath = str1 + str2 + ".zip";

				string despath = files[i];
				pos = files[i].rfind("/");
				despath = despath.substr(pos + 1, files[i].length());

				HZIP hz = CreateZip(zippath.c_str(), 0);
				ZipAdd(hz, despath.c_str(), files[i].c_str());
				CloseZip(hz);
				sent_file_name(zippath);
				
				file_transmision(zippath);
				remove(zippath.c_str());
				//���ͽ�ѹָ��
				
				string file_len = to_string(str3.length());
				while (file_len.length() < 4)
				{
					file_len = "0" + file_len;
				}

				int send_len;
				if (send_len = send(s_server, ("005" + file_len + str3).c_str(), ("005" + file_len + str3).length(), 0))
				{
					cout << "���ͽ�ѹָ��ɹ�" << endl;
				}
				else
				{
					cout << "���ͽ�ѹָ��ʧ��" << endl;
				}
				

			}
			else
			{

				if (!sent_file_name(files[i]))
				{
					re__connect();
					return;
				}
				
				if (!file_transmision(files[i]))
				{
					re__connect();
					return;
				}
			}
			
			//��¼�������
			fout.open("record.txt");
			fout << "Unfinished" << endl;
			fout << i;
			fout.close();

			//��¼�Ѿ�������ļ���
			fout1.open("sended.txt", ios::app);
			fout1 << files[i] << endl;
			fout1.close();

			//��¼���ļ�������޸�ʱ��
			fout3.open("time.txt", ios::app);
			string modify_time = GetFileTime(files[i]);
			fout3 << modify_time << endl;
			fout3.close();

		}
	}
	//Sleep(1);
	//Sleep(0.5);
	Sleep(20);
	if (!sent_end_transmission())
	{
		re__connect();
		return;
	}
	cout << "�ļ��������" << endl;

	//����������ϣ���¼
	fout.open("record.txt");
	fout << "Finished";
	fout.close();

	ends_t = clock();
	int pass_t = ends_t - start_t;
	cout << "����ʱ��" << (double)pass_t / 1000.0 << endl;

	flag = 1;//�������
}

void Client::__exit()
{
	//�ر��׽���
	closesocket(s_server);	//closesocket(s_server);
	//�ͷ�DLL��Դ
	WSACleanup();
}


void Client::set_file_length(char* ch, int len)
{
	/*string file_len = to_string(len);
	while (file_len.length() < 4)
	{
		file_len = "0" + file_len;
	}
	for (int i = 0; i < 4; i++)
	{
		ch[3 + i] = file_len[i];
	}*/
	int temp_len = len;
	for (int i = 6; i >= 3; i--)
	{
		ch[i] = (temp_len % 10)+48;
		temp_len /= 10;
	}
}
