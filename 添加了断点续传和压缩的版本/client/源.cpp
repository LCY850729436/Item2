#include"Client.h"

int main()
{
	int flag = 0;//���崫�������־
	Client::__connect();
	while (flag == 0)Client::__trans(flag);
	Client::__exit();
	system("pause");
	return 0;
}