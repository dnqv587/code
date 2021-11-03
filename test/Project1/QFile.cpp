
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>



int _2main()
{
	int jsonID = 3;

	QString strAll;
	QStringList strList;
	int channelType = 0;// 1遥信;2遥测;3遥控
	QFile readFile("C:/Users/UK2021/Documents/test/Project1/test.txt");	
	if (QFile::exists("C:/Users/UK2021/Documents/test/Project1/test.txt") == false)//文件不存在就创建
	{

		readFile.open(QIODevice::ReadWrite | QIODevice::Text);

	}
	if (readFile.open((QIODevice::ReadOnly | QIODevice::Text)))
	{
		//把文件所有信息读出来
		QTextStream stream(&readFile);
		stream.setCodec("utf-8");
		strAll = stream.readAll();
	}
	readFile.close();
	QString str = u8"我的界";
	QFile writeFile("C:/Users/UK2021/Documents/test/Project1/test.txt");
	if (writeFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&writeFile);
		stream.setCodec("utf-8");
		strList = strAll.split("\n");           //以换行符为基准分割文本
		for (int i = 0; i < strList.count(); i++)    //遍历每一行
		{
			QString pstr = strList.at(i);
			QString ID,jID;
			QString value;
			
			if (strList.at(i) == QString::fromLocal8Bit("[遥信]"))
				channelType = 1;
			else if (strList.at(i) == QString::fromLocal8Bit("[遥测]"))
				channelType = 2;
			else if (strList.at(i) == QString::fromLocal8Bit("[遥控]"))
				channelType = 3;
			
			for (int i = 0; i < pstr.length(); i++)
			{
				if (pstr.at(i) == ",")
					break;
				ID += pstr.at(i);
			}
			if (ID== jID.setNum(jsonID,10) && channelType == 2)    //"是要修改的内容	
			{
				QString tempStr = strList.at(i);
				value = ID + "," + str;
				tempStr.replace(0, tempStr.length(), value);   //要替换的内容
				stream << tempStr << '\n';
			}
			//如果没有找到要替换的内容，照常写入
			else
			{
				if (i == strList.count() - 1)
				{
					stream << strList.at(i);
				}
				else
				{
					stream << strList.at(i) << '\n';
				}
			}
		}
	}
	writeFile.close();
	return 0;
}

int main()
{
	QFile file("C:/Users/UK2021/Documents/test/Project1/test.txt");
	if (file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream.setCodec("utf-8");
		QString str = QString::fromLocal8Bit("[遥测]");
		stream << "TagId,TagName" << '\n';
		stream << str << '\n';
	}


}