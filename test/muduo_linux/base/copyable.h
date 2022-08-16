#pragma once
/*
其派生类继承此基类，将声明派生类可拷贝
*/
class copyable
{
protected:
	copyable() = default;
	~copyable() = default;
};
