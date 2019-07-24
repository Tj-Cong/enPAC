#include "xml2ltl.h"
#include<iostream>
using namespace std;

void GetLTLf(TiXmlElement *p, ofstream &outltl)
{
	string Value = p->Value();
	if (Value == "all-paths")
	{
		GetLTLf(p->FirstChildElement(), outltl);
	}
	else if (Value == "next")
	{
		outltl << "X(";
		GetLTLf(p->FirstChildElement(), outltl);
		outltl << ")";
	}
	else if (Value == "globally")
	{
		outltl << "[](";
		GetLTLf(p->FirstChildElement(), outltl);
		outltl << ")";
	}
	else if (Value == "until")
	{
		TiXmlElement *m, *n;
		m = p->FirstChildElement();
		n = m->NextSiblingElement();
		string mValue = m->Value();
		string nValue = n->Value();
		if (mValue != "before")
		{
			cerr << "Error in XML file! The element until\'s first child is not before!" << endl;
			exit(-1);
		}
		if (nValue!= "reach")
		{
			cerr << "Error in XML file! The element until\'s second child is not reach!" << endl;
			exit(-1);
		}
		outltl << "(";
		GetLTLf(m->FirstChildElement(), outltl);
		outltl << ")U(";
		GetLTLf(n->FirstChildElement(), outltl);
		outltl << ")";
	}
	else if (Value == "finally")
	{
		outltl << "<>(";
		GetLTLf(p->FirstChildElement(), outltl);
		outltl << ")";
	}
	else if (Value == "is-fireable")
	{
		TiXmlElement *m;
		m = p->FirstChildElement();
		outltl << "{";
		while (m!=NULL)
		{
			string Value = m->Value();
			if (Value == "transition")
			{
				outltl << m->GetText() << ",";
			}
			else
			{
				cerr << "Error in XML file! One of the element is-firability\'s child is not transition!" << endl;
				exit(-1);
			}
			m = m->NextSiblingElement();
		}
		outltl << "}";
	}
}
void GetLTLC(TiXmlElement *p, ofstream &outltl)
{
	string value = p->Value();
	if (value == "all-paths")
	{
		GetLTLC(p->FirstChildElement(), outltl);
	}
	else if (value == "globally")
	{
		outltl << "[](";
		GetLTLC(p->FirstChildElement(), outltl);
		outltl << ")";
	}
	else if (value == "finally")
	{
		outltl << "<>(";
		GetLTLC(p->FirstChildElement(), outltl);
		outltl << ")";
	}
	else if (value == "next")
	{
		outltl << "X(";
		GetLTLC(p->FirstChildElement(), outltl);
		outltl << ")";
	}
	else if (value == "until")
	{
		TiXmlElement *m, *n;
		m = p->FirstChildElement();
		n = m->NextSiblingElement();
		string mValue = m->Value();
		string nValue = n->Value();
		if (mValue != "before")
		{
			cerr << "Error in XML file! The element until\'s first child is not before!" << endl;
			exit(-1);
		}
		if (nValue != "reach")
		{
			cerr << "Error in XML file! The element until\'s second child is not reach!" << endl;
			exit(-1);
		}
		outltl << "(";
		GetLTLC(m->FirstChildElement(), outltl);
		outltl << ")U(";
		GetLTLC(n->FirstChildElement(), outltl);
		outltl << ")";
	}
	else if (value == "integer-le")
	{
		TiXmlElement *m, *n;
		m = p->FirstChildElement();
		n = m->NextSiblingElement();
		string mValue = m->Value();
		string nValue = n->Value();
		outltl << "{";
		if (mValue == "tokens-count")
		{
			outltl << "token-count(";
			TiXmlElement *left = m->FirstChildElement();
			while (left != NULL)
			{
				outltl << left->GetText() << ",";
				left = left->NextSiblingElement();
			}
			outltl << ")";
		}
		else if (mValue == "integer-constant")
		{
			outltl << m->GetText();
		}
		else {
			cerr << "Error in XML file about the element integer-le!" << endl;
			exit(-1);
		}
		outltl << "<=";
		if (nValue == "tokens-count")
		{
			outltl << "token-count(";
			TiXmlElement *right = n->FirstChildElement();
			while (right != NULL)
			{
				outltl << right->GetText() << ",";
				right = right->NextSiblingElement();
			}
			outltl << ")";
		}
		else if (mValue == "integer-constant")
		{
			outltl << n->GetText();
		}
		else {
			cerr << "Error in XML file about the element integer-le!" << endl;
			exit(-1);
		}
		outltl << "}";
	}
}
void convertf(char *filename)
{
	ofstream outltl("LTLFireability.txt", ios::out);
	TiXmlDocument *doc = new TiXmlDocument(filename);
	if (!doc->LoadFile())
	{
		cerr << doc->ErrorDesc() << endl;	
	}
	TiXmlElement *root= doc->RootElement();
	if (root == NULL)
	{
		cerr << "Failed to load file: no root element!" << endl;
		doc->Clear();
	}
	TiXmlElement *p = root->FirstChildElement();
	while (p!=NULL)
	{
		TiXmlElement *m = p->FirstChildElement();
		string Value = m->Value();
		outltl << m->GetText() <<":";
		while (Value!="formula")
		{
			m = m->NextSiblingElement();
			Value = m->Value();
		}
		m = m->FirstChildElement();
		GetLTLf(m->FirstChildElement(), outltl);
		outltl << endl;
		p = p->NextSiblingElement();
	}
}
void convertc(char *filename)
{
	ofstream outltl("LTLCardinality.txt", ios::out);
	TiXmlDocument *doc = new TiXmlDocument(filename);
	if (!doc->LoadFile())
	{
		cerr << doc->ErrorDesc() << endl;
	}
	TiXmlElement *root = doc->RootElement();
	if (root == NULL)
	{
		cerr << "Failed to load file: no root element!" << endl;
		doc->Clear();
	}
	TiXmlElement *p = root->FirstChildElement();
	while (p != NULL)
	{
		TiXmlElement *id = p->FirstChildElement("id");
		outltl << id->GetText() << ":";
		TiXmlElement *formula = p->FirstChildElement("formula");
		GetLTLC(formula->FirstChildElement(), outltl);
		outltl << endl;
		p = p->NextSiblingElement();
	}
}
