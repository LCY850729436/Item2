#include"Server.h"
#include"Delete.h"
#include"unzip.h"
#include"zip.h"

SOCKET Server::s_server;
SOCKET Server::s_accept;
//����˵�ַ�ͻ��˵�ַ
SOCKADDR_IN Server::server_addr;
SOCKADDR_IN Server::accept_addr;

string Server::file_path;
int port;
string server_ip;

ifstream fin;
ofstream fout;
ofstream fout1;

queue<char*> Server::infor_buf;
queue<int> Server::infor_len_buf;
bool Server::is_end = false;
string lastfile;
string firstfile;


void Server::initialization()
{
    //��ʼ���׽��ֿ�
    WORD w_req = MAKEWORD(2, 2);//�汾��
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0) {
        cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << endl;
    }
    else {
        cout << "��ʼ���׽��ֿ�ɹ���" << endl;
    }
    //���汾��
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
        cout << "�׽��ֿ�汾�Ų�����" << endl;
        WSACleanup();
    }
    else {
        cout << "�׽��ֿ�汾��ȷ��" << endl;
    }
}

int Server::get_operate_number(const char* buf)
{
    return (buf[0] - 48) * 100 + (buf[1] - 48) * 10 + (buf[2] - 48);
}

void Server::__start()
{
    int len;
    initialization();

    cout << "�����롾�ļ���ַ�� ��ip�� ���˿ںš��м���һ���ո����" << endl;
    cin >> file_path >> server_ip >> port;
    cout << "�������Ѿ�����" << endl;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip.c_str()); //htonl(INADDR_ANY);192.168.31.212
    server_addr.sin_port = htons(port);	//8307
    //�����׽���	
    s_server = socket(AF_INET, SOCK_STREAM, 0);
    if (::bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        cout << "�׽��ְ�ʧ�ܣ�" << endl;		WSACleanup();
    }
    else
    {
        cout << "�׽��ְ󶨳ɹ���" << endl;
    }
    //�����׽���Ϊ����״̬	
    if (listen(s_server, SOMAXCONN) < 0)
    {
        cout << "���ü���״̬ʧ�ܣ�" << endl;
        WSACleanup();
    }
    else
    {
        cout << "���ü���״̬�ɹ���" << endl;
    }
    cout << "��������ڼ������ӣ����Ժ�...." << endl;
    //������������
    len = sizeof(SOCKADDR);
    s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	while (s_accept == SOCKET_ERROR)
	{
		cout << "����ʧ�ܣ����ڳ�����������" << endl << endl;
		WSACleanup();
		Sleep(2000);
		initialization();
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip.c_str()); //htonl(INADDR_ANY);192.168.31.212
		server_addr.sin_port = htons(port);	//8307
		//�����׽���	
		s_server = socket(AF_INET, SOCK_STREAM, 0);
		if (::bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
		{
			cout << "�׽��ְ�ʧ�ܣ�" << endl;

			WSACleanup();
		}
		else cout << "�׽��ְ󶨳ɹ���" << endl;

		//�����׽���Ϊ����״̬	
		if (listen(s_server, SOMAXCONN) < 0)
		{
			cout << "���ü���״̬ʧ�ܣ�" << endl;
			WSACleanup();
		}
		else cout << "���ü���״̬�ɹ���" << endl;

		cout << "��������ڼ������ӣ����Ժ�...." << endl;
		//������������
		len = sizeof(SOCKADDR);
		s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	}
    cout << "���ӽ�����׼����������" << endl;
}

void Server::re__start()
{
	cout << "�ͻ��˺�������ܶϿ������ڳ�����������" << endl;
	WSACleanup();
	Sleep(2000);
	initialization();
	int len;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip.c_str()); //htonl(INADDR_ANY);192.168.31.212
	server_addr.sin_port = htons(port);	//8307
	//�����׽���	
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (::bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "�׽��ְ�ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else cout << "�׽��ְ󶨳ɹ���" << endl;

	//�����׽���Ϊ����״̬	
	if (listen(s_server, SOMAXCONN) < 0)
	{
		cout << "���ü���״̬ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else cout << "���ü���״̬�ɹ���" << endl;

	cout << "��������ڼ������ӣ����Ժ�...." << endl;
	//������������
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	while (s_accept == SOCKET_ERROR)
	{
		cout << "����ʧ�ܣ����ڳ�����������" << endl << endl;
		WSACleanup();
		Sleep(2000);
		initialization();
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip.c_str()); //htonl(INADDR_ANY);192.168.31.212
		server_addr.sin_port = htons(port);	//8307
		//�����׽���	
		s_server = socket(AF_INET, SOCK_STREAM, 0);
		if (::bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
		{
			cout << "�׽��ְ�ʧ�ܣ�" << endl;
			WSACleanup();
		}
		else cout << "�׽��ְ󶨳ɹ���" << endl;

		//�����׽���Ϊ����״̬	
		if (listen(s_server, SOMAXCONN) < 0)
		{
			cout << "���ü���״̬ʧ�ܣ�" << endl;
			WSACleanup();
		}
		else cout << "���ü���״̬�ɹ���" << endl;

		cout << "��������ڼ������ӣ����Ժ�...." << endl;
		//������������
		len = sizeof(SOCKADDR);
		s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	}
	cout << "�ɹ����½������ӣ�׼����������" << endl;
}

void Server::__trans(int &flag)
{
	ifstream fin1("ceived.txt");
	if (fin1.is_open())getline(fin1, firstfile);
	fin1.close();
    int send_len = 0;
    int recv_len = 0;
    //int len = 0;
    char recv_buf[100];

    if (_access(file_path.c_str(), 0) == -1)	//����ļ��в�����
    {
        cout << "�����ڸ��ļ��л����ļ�" << endl;
        exit(-1);
    }

    if (file_path[file_path.length() - 1] != '/' && file_path[file_path.length() - 1] != '\\')
    {
        file_path += '/';
    }


    //thread t_1(accept_files_from_the_host);
    //Sleep(10);
    if (send(s_accept, "start", 6, 0) < 0)
    {
        cout << "�������006" << endl;
		re__start();
        return;
    }
    //Sleep(10);
   // thread t_2(executive);
    //thread t_2(temp_test);
    //cout << "flag" << endl;

    //t_1.join();
    //t_2.join();
    //cout << "���д�С" << infor_buf.size() << endl;
    

    //analysis_test();


	if (!recv_from_client())//��ʼ���ܿͻ��˴���������ļ�
	{
		re__start();
		return;
	}

    //accept_files_from_the_host();
    cout << "�������" << endl;
	//������ϣ���¼���������Ϣ
	fout.open("record.txt");
	fout << "Finished";
	fout.close();

	flag = 1;
}

void Server::__end()
{
    //�ر��׽���
    closesocket(s_server);	closesocket(s_accept);
    //�ͷ�DLL��Դ
    WSACleanup();
}

int Server::get_messeage_len(const char* recv_current_m_len)
{
    int m_len = 0;
    for (int i = 0; i < 4; i++)
    {
        m_len = m_len * 10 + recv_current_m_len[i] - 48;
    }
    return m_len;
}


bool Server::recv_from_client()
{
    //int recv_len;
    char* messeage = NULL;
    int messeage_len = 0;
    ofstream outfile;
    int current_len = 0;
    int each_packet_len = 0;
    Server::State current_state = Server::State::complete_end;
    char operation_order[3];
    int order;
    char recv_current_m_len[4];
    int remaining_len = 0;

    string file_packet_name;
    string single_file_name;

    while (true)
    {
        char* messeage = new char[SERVER_ACCEPT_BUFFER_SIZE];//1028
        //char* buf = (char*)malloc(sizeof(char) * SERVER_ACCEPT_BUFFER_SIZE);
        messeage_len = recv(s_accept, messeage, SERVER_ACCEPT_BUFFER_SIZE, 0);
		if (messeage_len < 0)
		{
			cout << "����ʧ��" << endl;
			return false;
		}

        current_len = 0;
        char* messeage_copy = messeage;

        while (messeage_len - current_len > 0)
        {
            if (current_state == State::complete_end)
            {
                if (messeage_len - current_len >= HEAD_SIZE)
                {
                    operation_order[0] = messeage[0]; operation_order[1] = messeage[1]; operation_order[2] = messeage[2];
                    recv_current_m_len[0] = messeage[3]; recv_current_m_len[1] = messeage[4];
                    recv_current_m_len[2] = messeage[5]; recv_current_m_len[3] = messeage[6];
                    order = get_operate_number(operation_order);
                    each_packet_len = get_messeage_len(recv_current_m_len);
                    messeage += HEAD_SIZE;
                    current_len += HEAD_SIZE;
                }
                else
                {
                    remaining_len = messeage_len - current_len;
                    for (int i = 0; i < remaining_len; i++)
                    {
                        if (i < 3)
                        {
                            operation_order[i] = messeage[i];
                        }
                        else
                        {
                            recv_current_m_len[i - 3] = messeage[i];
                        }
                    }
                    messeage += messeage_len - current_len;
                    current_len += messeage_len - current_len;
                    current_state = State::order_break;
                    continue;
                }

                if (messeage_len - current_len >= each_packet_len)
                {
                    switch (order)
                    {
                    case 0:
                    {
                        cout << "�������ݽ������" << endl;
						return true;
                        break;
                    }
                    case 1:
                    {
                        outfile.write(messeage, each_packet_len);
                        //current_state = State::complete_end;
                        break;
                    }
                    case 2:
                    {
                        outfile.close();
                        break;
                    }
                    case 3:
                    {
						int flag = 0;//Ϊ���ʾû���жϣ�Ϊ1��ʾ�ϴδ��䱻�ж�
						fin.open("record.txt");
						//������ļ������ڣ���˵���ǵ�һ�δ���
						if (!fin.is_open())
						{
							fin.close();
							//�����ü�¼����������ļ�
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}
						else
						{
							string info;
							getline(fin, info);
							if (info != "Finished")flag = 1;
							fin.close();
						}
                        string prefix = file_path;
                        for (int i = 0; i < each_packet_len; i++)
                        {
                            prefix += messeage[i];
                        }
                        if (_access(prefix.c_str(), 0) == -1)	//����ļ��в�����
                            _mkdir(prefix.c_str());
						if (flag == 0)
						{
							fout1.open("ceived.txt");
							fout1 << prefix << endl;
							fout1.close();
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}
						else
						{
							fout1.open("ceived.txt", ios::app);
							fout1 << prefix << endl;
							fout1.close();
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}
                        break;
                    }
                    case 4:
                    {
						int flag = 0;//Ϊ���ʾû���жϣ�Ϊ1��ʾ�ϴδ��䱻�ж�
						fin.open("record.txt");
						//������ļ������ڣ���˵���ǵ�һ�δ���
						if (!fin.is_open())
						{
							fin.close();
							//�����ü�¼����������ļ�
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}
						else
						{
							string info;
							getline(fin, info);
							if (info != "Finished")flag = 1;
							fin.close();
						}
                        string file_name = file_path;
                        for (int i = 0; i < each_packet_len; i++)
                        {
                            file_name += messeage[i];
                        }
                        cout << "���ļ�" << file_name << "����д��" << endl;
						lastfile = file_name;
                        outfile.open(file_name, ios::out | ios::binary);
						if (flag == 0)
						{
							fout1 << file_name << endl;
							fout1.close();
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}
						else
						{
							fout1.open("ceived.txt", ios::app);
							fout1 << file_name << endl;
							fout1.close();
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}
                        break;
                    }
					//��ѹ���͹����ı�ѹ�����ļ�
					case 5:
					{
						HZIP hz = OpenZip(lastfile.c_str(), 0);

						string str1 = lastfile;
						int pos = str1.rfind("/");
						str1 = str1.erase(pos, str1.length());

						SetUnzipBaseDir(hz, str1.c_str());

						ZIPENTRY ze;
						GetZipItem(hz, -1, &ze);
						int numitems = ze.index;
						for (int zi = 0; zi < numitems; zi++)
						{
							ZIPENTRY ze;
							GetZipItem(hz, zi, &ze);
							UnzipItem(hz, zi, ze.name);
						}
						CloseZip(hz);
						remove(lastfile.c_str());

						cout << "��ѹ�ɹ�" << str1 << "�ɹ�" << endl;

						break;
					}
					case 111:
					{

						cout << "����ɾ���ϴ��Ѿ�����õ��ļ�" << endl << endl;
						DeleteAllFile(firstfile);
						_rmdir(firstfile.c_str());
						cout << "ɾ���ɹ�" << firstfile << endl << endl;

						fout1.open("ceived.txt");
						fout1.close();

						break;
					}
                    default:
                    {
                        cout << "����û�����������   " << order << "    2020/4/19" << endl;
                        break;
                    }
                    }
                    current_state = State::complete_end;
                    messeage += each_packet_len;
                    current_len += each_packet_len;
                }
                else
                {
                    //int remaining_len = messeage_len - current_len;
                    remaining_len = messeage_len - current_len;
                    switch (order)
                    {
                    case 0:
                    {
                        cout << "�������ݽ������" << endl;
						return true;
                        break;
                    }
                    case 1:
                    {
                        outfile.write(messeage, remaining_len);
                        //current_state = State::complete_end;
                        break;
                    }
                    case 2:
                    {
                        //outfile.close();
                        //֮���ٹ�
                        break;
                    }
                    case 3:
                    {
                        file_packet_name = file_path;
                        for (int i = 0; i < remaining_len; i++)
                        {
                            file_packet_name += messeage[i];
                        }

                        break;
                    }
                    case 4:
                    {
                        single_file_name = file_path;
                        for (int i = 0; i < remaining_len; i++)
                        {
                            single_file_name += messeage[i];
                        }
                        //cout << "���ļ�" << file_name << "����д��" << endl;
                       // outfile.open(file_name, ios::out | ios::binary);
                        break;
                    }
					//��ѹ���͹����ı�ѹ�����ļ�
					case 5:
					{
						HZIP hz = OpenZip(lastfile.c_str(), 0);

						string str1 = lastfile;
						int pos = str1.rfind("/");
						str1 = str1.erase(pos, str1.length());

						SetUnzipBaseDir(hz, str1.c_str());

						ZIPENTRY ze;
						GetZipItem(hz, -1, &ze);
						int numitems = ze.index;
						for (int zi = 0; zi < numitems; zi++)
						{
							ZIPENTRY ze;
							GetZipItem(hz, zi, &ze);
							UnzipItem(hz, zi, ze.name);
						}
						CloseZip(hz);
						remove(lastfile.c_str());

						cout << "��ѹ�ɹ�" << str1 << "�ɹ�" << endl;

						break;
					}
					case 111:
					{

						cout << "����ɾ���ϴ��Ѿ�����õ��ļ�" << endl << endl;
						DeleteAllFile(firstfile);
						_rmdir(firstfile.c_str());
						cout << "ɾ���ɹ�" << firstfile << endl << endl;

						fout1.open("ceived.txt");
						fout1.close();

						break;
					}
                    default:
                    {
                        cout << "����û�����������   " << order << "    2020/4/19" << endl;
                        break;
                    }
                    }

                    current_len = messeage_len;
                    messeage += messeage_len - current_len;
                    current_state = State::infor_break;
                    continue;
                }
            }
            else if (current_state == State::infor_break)
            {
                if (each_packet_len - remaining_len <= messeage_len)
                {
                    switch (order)
                    {
                    case 0:
                    {
                        cout << "�������ݽ������" << endl;
						return true;
                        break;
                    }
                    case 1:
                    {
                        outfile.write(messeage, each_packet_len - remaining_len);
                        //current_state = State::complete_end;
                        break;
                    }
                    case 2:
                    {
                        outfile.close();//����ر�
                        break;
                    }
                    case 3:
                    {
						int flag = 0;//Ϊ���ʾû���жϣ�Ϊ1��ʾ�ϴδ��䱻�ж�
						fin.open("record.txt");
						//������ļ������ڣ���˵���ǵ�һ�δ���
						if (!fin.is_open())
						{
							fin.close();
							//�����ü�¼����������ļ�
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}
						else
						{
							string info;
							getline(fin, info);
							if (info != "Finished")flag = 1;
							fin.close();
						}

						for (int i = 0; i < each_packet_len - remaining_len; i++)
						{
							file_packet_name += messeage[i];
						}
						if (_access(file_packet_name.c_str(), 0) == -1)	//����ļ��в�����
							_mkdir(file_packet_name.c_str());

						if (flag == 0)
						{
							fout1.open("ceived.txt");
							fout1 << file_packet_name << endl;
							fout1.close();
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}
						else
						{
							fout1.open("ceived.txt", ios::app);
							fout1 << file_packet_name << endl;
							fout1.close();
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}

                        break;
                    }
                    case 4:
                    {
						int flag = 0;//Ϊ���ʾû���жϣ�Ϊ1��ʾ�ϴδ��䱻�ж�
						fin.open("record.txt");
						//������ļ������ڣ���˵���ǵ�һ�δ���
						if (!fin.is_open())
						{
							fin.close();
							//�����ü�¼����������ļ�
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}
						else
						{
							string info;
							getline(fin, info);
							if (info != "Finished")flag = 1;
							fin.close();
						}

						//single_file_name = "";
						for (int i = 0; i < each_packet_len - remaining_len; i++)
						{
							single_file_name += messeage[i];
						}
						cout << "���ļ�" << single_file_name << "����д��" << endl;
						lastfile = single_file_name;
						outfile.open(single_file_name, ios::out | ios::binary);

						if (flag == 0)
						{
							fout1 << single_file_name << endl;
							fout1.close();
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}
						else
						{
							fout1.open("ceived.txt", ios::app);
							fout1 << single_file_name << endl;
							fout1.close();
							fout.open("record.txt");
							fout << "Unfinish";
							fout.close();
						}
                        break;
                    }
					//��ѹ���͹����ı�ѹ�����ļ�
					case 5:
					{
						HZIP hz = OpenZip(lastfile.c_str(), 0);

						string str1 = lastfile;
						int pos = str1.rfind("/");
						str1 = str1.erase(pos, str1.length());

						SetUnzipBaseDir(hz, str1.c_str());

						ZIPENTRY ze;
						GetZipItem(hz, -1, &ze);
						int numitems = ze.index;
						for (int zi = 0; zi < numitems; zi++)
						{
							ZIPENTRY ze;
							GetZipItem(hz, zi, &ze);
							UnzipItem(hz, zi, ze.name);
						}
						CloseZip(hz);
						remove(lastfile.c_str());

						cout << "��ѹ�ɹ�" << str1 << "�ɹ�" << endl;

						break;
					}
					case 111:
					{

						cout << "����ɾ���ϴ��Ѿ�����õ��ļ�" << endl << endl;
						DeleteAllFile(firstfile);
						_rmdir(firstfile.c_str());
						cout << "ɾ���ɹ�" << firstfile << endl << endl;

						fout1.open("ceived.txt");
						fout1.close();

						break;
					}
                    default:
                    {
                        cout << "����û�����������   " << order << "    2020/4/19" << endl;
                        break;
                    }
                    }

                    messeage += each_packet_len - remaining_len;
                    current_len += each_packet_len - remaining_len;
                    current_state = State::complete_end;
                }
                else
                {
                    cout << "��������1" << endl;
                    switch (order)
                    {
                    case 0:
                    {
                        cout << "�������ݽ������" << endl;
						return true;
                        break;
                    }
                    case 1:
                    {
                        outfile.write(messeage, messeage_len);
                        //current_state = State::complete_end;
                        break;
                    }
                    case 2:
                    {
                        //outfile.close();
                        //֮���ٹ�
                        break;
                    }
                    case 3:
                    {
                        //file_packet_name = "";
                        for (int i = 0; i < messeage_len; i++)
                        {
                            file_packet_name += messeage[i];
                        }

                        break;
                    }
                    case 4:
                    {
                        //single_file_name = "";
                        for (int i = 0; i < messeage_len; i++)
                        {
                            single_file_name += messeage[i];
                        }
                        //cout << "���ļ�" << file_name << "����д��" << endl;
                       // outfile.open(file_name, ios::out | ios::binary);
                        break;
                    }
					//��ѹ���͹����ı�ѹ�����ļ�
					case 5:
					{
						HZIP hz = OpenZip(lastfile.c_str(), 0);

						string str1 = lastfile;
						int pos = str1.rfind("/");
						str1 = str1.erase(pos, str1.length());

						SetUnzipBaseDir(hz, str1.c_str());

						ZIPENTRY ze;
						GetZipItem(hz, -1, &ze);
						int numitems = ze.index;
						for (int zi = 0; zi < numitems; zi++)
						{
							ZIPENTRY ze;
							GetZipItem(hz, zi, &ze);
							UnzipItem(hz, zi, ze.name);
						}
						CloseZip(hz);
						remove(lastfile.c_str());

						cout << "��ѹ�ɹ�" << str1 << "�ɹ�" << endl;

						break;
					}
					case 111:
					{

						cout << "����ɾ���ϴ��Ѿ�����õ��ļ�" << endl << endl;
						DeleteAllFile(firstfile);
						_rmdir(firstfile.c_str());
						cout << "ɾ���ɹ�" << firstfile << endl << endl;

						fout1.open("ceived.txt");
						fout1.close();

						break;
					}
                    default:
                    {
                        cout << "����û�����������   " << order << "    2020/4/19" << endl;
                        break;
                    }
                    }

                    current_len = messeage_len;
                    messeage += messeage_len;
                    remaining_len += messeage_len;
                    current_state = State::infor_break;
                }
            }
            else if (current_state == State::order_break)
            {
                if (messeage_len >= HEAD_SIZE - remaining_len)
                {
                    for (int i = remaining_len; i < HEAD_SIZE; i++)
                    {
                        if (i < 3)
                        {
                            operation_order[i] = messeage[i - remaining_len];
                        }
                        else
                        {
                            recv_current_m_len[i - 3] = messeage[i - remaining_len];
                        }
                    }
                    current_len += HEAD_SIZE - remaining_len;
                    messeage += HEAD_SIZE - remaining_len;
                    order = get_operate_number(operation_order);
                    each_packet_len = get_messeage_len(recv_current_m_len);
                    if (messeage_len - current_len >= each_packet_len)
                    {
                        switch (order)
                        {
                        case 0:
                        {
                            cout << "�������ݽ������" << endl;
							return true;
                            break;
                        }
                        case 1:
                        {
                            outfile.write(messeage, each_packet_len);
                            //current_state = State::complete_end;
                            break;
                        }
                        case 2:
                        {
                            outfile.close();
                            break;
                        }
                        case 3:
                        {
							int flag = 0;//Ϊ���ʾû���жϣ�Ϊ1��ʾ�ϴδ��䱻�ж�
							fin.open("record.txt");
							//������ļ������ڣ���˵���ǵ�һ�δ���
							if (!fin.is_open())
							{
								fin.close();
								//�����ü�¼����������ļ�
								fout.open("record.txt");
								fout << "Unfinish";
								fout.close();
							}
							else
							{
								string info;
								getline(fin, info);
								if (info != "Finished")flag = 1;
								fin.close();
							}

							string prefix = file_path;
							for (int i = 0; i < each_packet_len; i++)
							{
								prefix += messeage[i];
							}
							if (_access(prefix.c_str(), 0) == -1)	//����ļ��в�����
								_mkdir(prefix.c_str());

							if (flag == 0)
							{
								fout1.open("ceived.txt");
								fout1 << prefix << endl;
								fout1.close();
								fout.open("record.txt");
								fout << "Unfinish";
								fout.close();
							}
							else
							{
								fout1.open("ceived.txt", ios::app);
								fout1 << prefix << endl;
								fout1.close();
								fout.open("record.txt");
								fout << "Unfinish";
								fout.close();
							}                        
                            break;
                        }
                        case 4:
                        {
							int flag = 0;//Ϊ���ʾû���жϣ�Ϊ1��ʾ�ϴδ��䱻�ж�
							fin.open("record.txt");
							//������ļ������ڣ���˵���ǵ�һ�δ���
							if (!fin.is_open())
							{
								fin.close();
								//�����ü�¼����������ļ�
								fout.open("record.txt");
								fout << "Unfinish";
								fout.close();
							}
							else
							{
								string info;
								getline(fin, info);
								if (info != "Finished")flag = 1;
								fin.close();
							}
							string file_name = file_path;
							for (int i = 0; i < each_packet_len; i++)
							{
								file_name += messeage[i];
							}
							cout << "���ļ�" << file_name << "����д��" << endl;
							lastfile = file_name;
							outfile.open(file_name, ios::out | ios::binary);
							if (flag == 0)
							{
								fout1 << file_name << endl;
								fout1.close();
								fout.open("record.txt");
								fout << "Unfinish";
								fout.close();
							}
							else
							{
								fout1.open("ceived.txt", ios::app);
								fout1 << file_name << endl;
								fout1.close();
								fout.open("record.txt");
								fout << "Unfinish";
								fout.close();
							}
                            break;
                        }
						//��ѹ���͹����ı�ѹ�����ļ�
						case 5:
						{
							HZIP hz = OpenZip(lastfile.c_str(), 0);

							string str1 = lastfile;
							int pos = str1.rfind("/");
							str1 = str1.erase(pos, str1.length());

							SetUnzipBaseDir(hz, str1.c_str());

							ZIPENTRY ze;
							GetZipItem(hz, -1, &ze);
							int numitems = ze.index;
							for (int zi = 0; zi < numitems; zi++)
							{
								ZIPENTRY ze;
								GetZipItem(hz, zi, &ze);
								UnzipItem(hz, zi, ze.name);
							}
							CloseZip(hz);
							remove(lastfile.c_str());

							cout << "��ѹ�ɹ�" << str1 << "�ɹ�" << endl;

							break;
						}
						case 111:
						{

							cout << "����ɾ���ϴ��Ѿ�����õ��ļ�" << endl << endl;
							DeleteAllFile(firstfile);
							_rmdir(firstfile.c_str());
							cout << "ɾ���ɹ�" << firstfile << endl << endl;

							fout1.open("ceived.txt");
							fout1.close();

							break;
						}
                        default:
                        {
                            cout << "����û�����������   " << order << "    2020/4/19" << endl;
                            break;
                        }
                        }
                        current_state = State::complete_end;
                        messeage += each_packet_len;
                        current_len += each_packet_len;
                    }
                    else
                    {
                        //int remaining_len = messeage_len - current_len;
                        remaining_len = messeage_len - current_len;
                        switch (order)
                        {
                        case 0:
                        {
                            cout << "�������ݽ������" << endl;
							return true;
                            break;
                        }
                        case 1:
                        {
                            outfile.write(messeage, remaining_len);
                            //current_state = State::complete_end;
                            break;
                        }
                        case 2:
                        {
                            //outfile.close();
                            //֮���ٹ�
                            break;
                        }
                        case 3:
                        {
                            file_packet_name = file_path;
                            for (int i = 0; i < remaining_len; i++)
                            {
                                file_packet_name += messeage[i];
                            }
							
                            break;
                        }
                        case 4:
                        {
                            single_file_name = file_path;
                            for (int i = 0; i < remaining_len; i++)
                            {
                                single_file_name += messeage[i];
                            }
                            //cout << "���ļ�" << file_name << "����д��" << endl;
                           // outfile.open(file_name, ios::out | ios::binary);
                            break;
                        }
						//��ѹ���͹����ı�ѹ�����ļ�
						case 5:
						{
							HZIP hz = OpenZip(lastfile.c_str(), 0);

							string str1 = lastfile;
							int pos = str1.rfind("/");
							str1 = str1.erase(pos, str1.length());

							SetUnzipBaseDir(hz, str1.c_str());

							ZIPENTRY ze;
							GetZipItem(hz, -1, &ze);
							int numitems = ze.index;
							for (int zi = 0; zi < numitems; zi++)
							{
								ZIPENTRY ze;
								GetZipItem(hz, zi, &ze);
								UnzipItem(hz, zi, ze.name);
							}
							CloseZip(hz);
							remove(lastfile.c_str());

							cout << "��ѹ�ɹ�" << str1 << "�ɹ�" << endl;

							break;
						}
						case 111:
						{

							cout << "����ɾ���ϴ��Ѿ�����õ��ļ�" << endl << endl;
							DeleteAllFile(firstfile);
							_rmdir(firstfile.c_str());
							cout << "ɾ���ɹ�" << firstfile << endl << endl;

							fout1.open("ceived.txt");
							fout1.close();

							break;
						}
                        default:
                        {
                            cout << "����û�����������   " << order << "    2020/4/19" << endl;
                            break;
                        }
                        }

                        current_len = messeage_len;
                        messeage += messeage_len - current_len;
                        current_state = State::infor_break;
                        continue;
                    }
                }
                else
                {
                    cout << "����һ��" << endl;
                    for (int i = remaining_len; i < messeage_len + remaining_len; i++)
                    {
                        if (i < 3)
                        {
                            operation_order[i] = messeage[i - remaining_len];
                        }
                        else
                        {
                            recv_current_m_len[i - 3] = messeage[i - remaining_len];
                        }
                    }
                    remaining_len += messeage_len;
                    current_state = State::order_break;
                }
            }
        }
        delete(messeage_copy);
    }
	return true;
}