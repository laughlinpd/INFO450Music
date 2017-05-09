// Inventory.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;

class Inventory
{
	string Artist;
	string Song;
	int	Rating;
	Inventory *next;
public:
	Inventory();
	Inventory(string Art, string Sng, int Rat);
	void CaptureInventoryItem();
	void ShowInventoryItem();
	int SaveInventoryItem(ofstream &outfile);
	friend class InventoryList;
};

class InventoryList
{

	int numrecords;
	Inventory *head;
	Inventory *tail;

public:
	void SkipSong();
	void PlaySong();
	InventoryList();
	~InventoryList();
	void GetUserInput();
	void AddNodeToEnd(Inventory *newnode);
	void ShowInventoryList();
	int SaveInventoryList(string filename);
	int ReadInventoryList(string filename);
};

// default constructor - initialize empty
Inventory::Inventory()
{
	Artist = ""; Song = ""; Rating = 0;
	next = NULL;
}
// overload constructor inialize w/ values
Inventory::Inventory(string num, string name, int quantity)
{
	Artist = num;
	Song = name;
	Rating = quantity;
	next = NULL;
}
// Capture Item from end users
void Inventory::CaptureInventoryItem()
{
	cout << "Inventory Num? -->";
	getline(cin, Artist);
	cout << "Inventory Name?  -->";
	getline(cin, Song);
	cout << "Quanity? -->";
	cin >> Rating;
	cin.ignore();
	cin.clear();
}


// Display item to console
void Inventory::ShowInventoryItem()
{
	cout << "Now Playing: " << Song << " By: " << Artist << " Rating: " << Rating << endl;
}

// Save inventory item to file given input file stream
int Inventory::SaveInventoryItem(ofstream& outfile)
{
	if (outfile.is_open())
	{
		outfile << Artist << "|" << Song << "|" << Rating << endl;
		return 0;
	}
	else
		return WRITEERROR;
}

// Inventory LIst constructor -allocate default space for array
InventoryList::InventoryList()
{

	numrecords = 0;
	head = NULL;
	tail = NULL;

}
// deconstructor - free allocated memory
InventoryList::~InventoryList()
{
	Inventory *ptr = head;
	while (ptr != NULL)
	{
		head = ptr->next;
		delete ptr;
		ptr = head;
	}
}

void InventoryList::AddNodeToEnd(Inventory *newnode)
{
	if (head == NULL)
	{
		head = newnode;
		tail = newnode;
	}
	else
	{
		tail->next = newnode;
		tail = newnode;
	}
}

// get user input for the list
void InventoryList::GetUserInput()
{
	string answer = "Y";
	// tbd check to see if i have enought space
	cout << "enter item Y/N?" << endl;
	getline(cin, answer);
	while (answer == "Y" || answer == "y")
	{
		Inventory *nptr = new Inventory();
		nptr->CaptureInventoryItem();
		AddNodeToEnd(nptr);

		numrecords++;
		cout << "enter another item Y/N?" << endl;
		getline(cin, answer);
	}
}

// Show list to the console
void InventoryList::ShowInventoryList()
{
	Inventory *ptr;
	if (head == NULL)
		return;  // cuase its empty!

	while (ptr != NULL)
	{
		ptr->ShowInventoryItem();
		ptr = ptr->next;
	}
}

void InventoryList::PlaySong()
{
	Inventory *ptr;
	
	ptr = ptr->next;
	cout << "Now Playing" << ptr->Song << "By " << ptr->Artist << endl;
	
}

void InventoryList::SkipSong()
{
	int x;
	Inventory *ptr;
	cout << "how many songs would you like to skip?" << endl;
	cin >> x;
	cin.clear();
	cin.ignore();
	
	for (int i = 0; i < x; i++)
	{
		
		ptr = ptr->next;
	}
	cout << "Now Playing" << ptr->Song << "By " << ptr->Artist << endl;

}

void InventoryList::DeleteSong()
{

}

// Save entire list to file, given full-path file name
int InventoryList::SaveInventoryList(string filename)
{
	ofstream output(filename, ios::trunc);
	if (output)
	{
		Inventory *ptr = head;
		while (ptr != NULL)
		{
			ptr->SaveInventoryItem(output);
			ptr = ptr->next;
		}

		output.close();
	}
	else
	{
		return WRITEERROR;
	}
	return 0;
}
// Read inventory list from a file, given full-path filename
// If file not found or error, return error code
int InventoryList::ReadInventoryList(string filename)
{
	string inum, iname, iqty;
	ifstream infile(filename, ios::in);
	if (!infile)
	{
		cout << "File could not be opened for reading" << endl;
		return READERROR;
	}


	while (!infile.eof())
	{

		getline(infile, inum, '|');
		if (!inum.empty())
		{
			getline(infile, iname, '|');
			getline(infile, iqty);
			int qty = stoi(iqty);

			Inventory *newnode = new Inventory(inum, iname, qty);
			AddNodeToEnd(newnode);

			numrecords++;
		}
	}

	infile.close();
	return 0;
}


int main()
{
	string filename;
	InventoryList myInventory;
	string answer;
	int error;

	cout << "Please enter file path" << endl;
	getline(cin, filename);
	error = myInventory.ReadInventoryList(filename);
	if (error)
	{
		cout << "Cannot read inventory - continue creating new list? Y/N -->";
		getline(cin, answer);
		if (answer != "Y" && answer != "y")
		{
			return 1;
		}
	}

	
	char input;
	do
	{
		cout << "[D]elete, [S]kip, [P]lay, [Q]uit? ";
		cin >> input;
		cin.clear();
		cin.ignore();
		switch (input)
		{
		case 'd':
		case 'D':
			break;
		case 'p':
		case 'P':
			myInventory.PlaySong();
			break;
		case 's':
		case 'S':
			myInventory.SkipSong();
		default:
			break;
		}
	} while (input != 'q' && input != 'Q');
	//myInventory.GetUserInput();
	
	myInventory.ShowInventoryList();
	myInventory.SaveInventoryList(filename);
	
	system("PAUSE");
		return 0;
}