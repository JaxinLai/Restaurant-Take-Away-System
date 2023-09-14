// Restaurant.cpp : This file contains the 'main' function. Program execution begins and ends there.

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <mysql.h>
#include <string>
#include <time.h>
#include <atltime.h>  
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <sstream>
using namespace std;

MYSQL* conn = mysql_init(0);
MYSQL_RES* res;
MYSQL_ROW row;
MYSQL_RES* res2;
MYSQL_ROW row2;
int qstate;
static string ID;
static int Current_Order_ID;
static string Current_Status;
static int Same_OrderID;

int main();
void Register();
void customerLogin();
void staffLogin();
void showStaff();
void manageMenu();
void addMenu();
void deleteMenu();
void viewMenu();
void interfaceCustomer();
void showCustomer();
void NewOrder();
void checkOrder();
void checkOrderDetail();
void UpdateStatus();
void viewOrder();
void anotherMenu();
void showOrder();
void viewOrderDetail();
void checkReceiptOrder();
void printStatus();
void OrderReport();

void EstimatedDuration();
void UpdateEstimateDuration();
void PrintReceipt();

void ReportType();
void displayReport();
void dailyReport();
void monthlyReport();

/*---------------------Database Connection--------------------------*/
static void ConnectionFunction()
{
	char  server[20] = "127.0.0.1";
	char  user[20] = "root";
	char  psd[20] = "";
	char  dbName[1024] = "dbworkshop";
	unsigned short port = 3301;

	if (mysql_real_connect(conn, server, user, psd, dbName, port, NULL, 0) == 0)
	{
		printf("Error connecting to database: %s\n", mysql_error(conn));
	}
	else {
		puts("Database Connected");
	}
}


void RestaurantName()
{
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\t\t\tWelcome to Lay's Restaurant!" << endl;
	cout << "\t\t-----------------------------------------------" << endl;
}

void staffLogin()
{
	system("cls");
	string id;
	string Password;
	char ch;

	RestaurantName();
	cout << endl;
	cout << "\t\tPlease enter your Staff ID and Password...." << endl;
	cout << setw(20) << "\n\t\tStaff ID   : ";
	cin.ignore();
	getline(cin, id);
	cout << setw(20) << "\n\t\tPassword   : ";
	while ((ch = _getch()) != 13)
	{
		Password += ch;
		cout << '*';
	}
	string checkUser_query = "select * from staff where Staff_ID = '" + id + "' and Password = '" + Password + "'"; //check whether staff_ID and password correct or not
	const char* cu = checkUser_query.c_str();
	qstate = mysql_query(conn, cu);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (res->row_count == 1) //if staff_id true, login successfully
		{
			if (row = mysql_fetch_row(res))
				showStaff();
		}
		else
		{
			char again = ' ';
			cout << "\n\n\t\tInvalid UserID or Password. Please enter again (Y/N): ";
			cin >> again;
			while ((again != 'Y') && (again != 'y') && (again != 'N') && (again != 'n')) {
				cout << "\n\t\tInvalid choice. Please enter again:";
				cin >> again;
			}
			if (again == 'Y' || again == 'y')
				staffLogin();
			if (again == 'N' || again == 'n')
				main();
		}
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

	}


}

void customerLogin()
{
	system("cls");
	string customerID;
	char ch;
	char again;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\t\t\t        Customer Login" << endl;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << endl;
	cout << "\t\tPlease enter your CustomerID and Password...." << endl;
	cout << setw(20) << "\n\t\tCustomerID :  ";
	cin.ignore();
	getline(cin, customerID);
	cout << setw(20) << "\n\t\tPassword   :  ";
	string customerPassword;
	while ((ch = _getch()) != 13)
	{
		customerPassword += ch;
		cout << '*';
	}
	string checkUser_query = "select Customer_ID from customer where Customer_ID = '" + customerID + "' and Password = '" + customerPassword + "'";
	const char* cu = checkUser_query.c_str();
	qstate = mysql_query(conn, cu);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (res->row_count == 1)
		{
			while (row = mysql_fetch_row(res))
				ID = row[0]; //save customer id to ID 
			showCustomer();
		}
		else
		{
			char again = ' ';
			cout << "\n\n\t\tInvalid CustomerID or Password. Please enter again (Y/N): ";
			cin >> again;
			while ((again != 'Y') && (again != 'y') && (again != 'N') && (again != 'n')) {
				cout << "\n\t\tInvalid choice. Please enter again:";
				cin >> again;
			}
			if (again == 'Y' || again == 'y')
				customerLogin();
			if (again == 'N' || again == 'n')
				interfaceCustomer();
		}
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
}

void Register()
{
	system("cls");
	string Name;
	string DOB;
	string Contact;
	string Email;
	string Password;
	string City;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\t\t\t\tCustomer Registration" << endl;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\n\t\tREGISTER BELOW" << endl;
	cout << "\n\t\tFull Name     : ";
	cin.ignore(1, '\n');
	getline(cin, Name);
	cout << "\n\t\tDate of Birth : ";
	getline(cin, DOB);
	cout << "\n\t\tPhone Contact : ";
	getline(cin, Contact);
	cout << "\n\t\tEmail Address : ";
	getline(cin, Email);
	cout << "\n\t\tPassword      : ";
	getline(cin, Password);
	cout << "\n\t\tCity          : ";
	getline(cin, City);

	string checkUser_query = "select * from customer where Email = '" + Email + "'";   //check email already exists or not
	const char* cu = checkUser_query.c_str();
	qstate = mysql_query(conn, cu);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (res->row_count == 1) //if already exits, print below
		{
			cout << "\n\t\tUser has been registered. Press Enter to Try Again...";
			_getch();
			Register();
		}
		else
		{
			string insertCustomer_query = "insert into customer (Name, DOB, Contact, Email, Password,City) values ('" + Name + "', '" + DOB + "', '" + Contact + "', '" + Email + "', '" + Password + "','" + City + "')";
			const char* q = insertCustomer_query.c_str();
			qstate = mysql_query(conn, q);

			if (!qstate)
			{
				cout << "\n\t\tCONGRATULATIONS! YOU ARE NOW REGISTERED!" << endl;

				string checkCustomerID_query = "select LAST_INSERT_ID();"; //show order id to customer for login purpose
				const char* cu = checkCustomerID_query.c_str();
				qstate = mysql_query(conn, cu);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (row = mysql_fetch_row(res))
					{
						cout << "\n\t\tYour Customer ID is: " << row[0] << endl;
					}
				}
				cout << "\n\t\tWant to Log-in? Press enter...";
				_getch();
				customerLogin();
			}
			else
			{
				cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
			}
		}
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
}

void showStaff()
{
	system("cls");
	int choice;

	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\t\t\t\tStaff's Menu" << endl;
	cout << "\t\t-----------------------------------------------" << endl;

	cout << "\n\t\t1. Manage Order" << endl;
	cout << "\n\t\t2. Manage Menu" << endl;
	cout << "\n\t\t3. Display report" << endl;
	cout << "\n\t\t4. Logout" << endl;
	cout << "\n\t\tYour choice: ";
	cin >> choice;
	switch (choice)
	{
	case 1:
		checkOrder();
		break;
	case 2:
		manageMenu();
		break;
	case 3:
		ReportType();
		break;
	case 4:
		main();
		break;
	default:
		cout << "\n\t\tInvalid choice. Please enter again....";
		_getch();
		system("cls");
		showStaff();
		break;
	}
}

void checkOrder() 
{
	system("cls");
	const char separator = ' ';
	cout << "\t\t---------------------------------------------------------------------------------------------------------" << endl;
	cout << "\t\t                                             Order List" << endl;
	cout << "\t\t---------------------------------------------------------------------------------------------------------" << endl;

	qstate = mysql_query(conn, "select * from order_list ORDER BY Order_Status ASC");

	if (!qstate)
	{
		cout << "\t\t--------------------------------------------------------------------------------------------------------" << endl;
		cout << "\t\t▏" << left << setw(7) << setfill(separator) << "Order ID"
			<< left << " ▏" << setw(10) << setfill(separator) << "Customer ID" 
			<< left << " ▏" << setw(19) << setfill(separator) << "Order_DateTime" 
			<< left << " ▏" << setw(19) << setfill(separator) << "Payment_DateTime" 
			<< left << " ▏" << setw(8) << setfill(separator) << "Order_Status" 
			<< left << " ▏" << setw(8) << setfill(separator) << "Estimate_Duration" << " ▏"<<endl;
		cout << "\t\t--------------------------------------------------------------------------------------------------------" << endl;
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << "\t\t▏" << left << setw(8) << setfill(separator) << row[0] 
				<< left << " ▏" << setw(11) << setfill(separator) << row[1] 
				<< left << " ▏" << setw(9) << setfill(separator) << row[2]
				<< left << " ▏" << setw(10) << setfill(separator) << row[3] 
				<< left << " ▏" << setw(12) << setfill(separator) << row[4]
				<< left << " ▏    " << setw(13) << setfill(separator) << row[5] << " ▏"
				<< endl;
			cout << "\t\t--------------------------------------------------------------------------------------------------------" << endl;
		}
		cout << endl;
		int OrderID;
		cout << "\n\t\tOrderID  : ";
		cin >> OrderID;
		string order_query = "select * from order_list where Order_ID  = " + to_string(OrderID); //change integer to string
		const char* q = order_query.c_str();
		qstate = mysql_query(conn, q);
		if (!qstate) {
			res = mysql_store_result(conn);
			if (mysql_num_rows(res) > 0) //check order id 
			{
				Current_Order_ID = OrderID;
				string status_query = "select * from order_list where Order_ID  = " + to_string(OrderID);
				const char* q = status_query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					row = mysql_fetch_row(res);
					Current_Status = row[4];
					checkOrderDetail();
				}
			}
			else {
				cout << "\n\t\tNo OrderID " << endl;
				char choice = ' ';
				cout << "\n\t\tEnter OrderID Again (Y/N): ";
				cin >> choice;
				while ((choice != 'Y') && (choice != 'y') && (choice != 'N') && (choice != 'n')) {
					cout << "\n\t\tInvalid choice. Please enter again:";
					cin >> choice;
				}
				if (choice == 'Y' || choice == 'y')
					checkOrder();
				if (choice == 'N' || choice == 'n')
					showStaff();
			}
		}
		else
			cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;

	}
	else
	{
		cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;

	}

}

void checkOrderDetail() 
{ 
	system("cls");
	const char separator = ' ';
	cout << "\t\t----------------------------------------------------------------" << endl;
	cout << "\t\t\t                  ORDER ID: " << Current_Order_ID << endl;
	cout << "\t\t----------------------------------------------------------------" << endl;
	string order_query = "select * from orderlistdetail where Order_ID=" + to_string(Current_Order_ID);
	const char* q = order_query.c_str();
	qstate = mysql_query(conn, q);
	if (!qstate)
	{
		cout << "\t\t----------------------------------------------------------------" << endl;

		cout << "\t\t▏" << left << setw(30) << setfill(separator) << "Menu"
			<< left << " ▏" << setw(9) << setfill(separator) << "Quantity"
			<< left << " ▏" << setw(4) << setfill(separator) << "Total_Price" << "\t▏"
			<< endl;

		cout << "\t\t----------------------------------------------------------------" << endl;
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			string menu_id = row[2];
			string select_query = "select Name from menu where Menu_ID='" + menu_id + "'";
			const char* q = select_query.c_str();
			mysql_query(conn, q);
			res2 = mysql_store_result(conn);
			row2 = mysql_fetch_row(res2);
			{
				cout << "\t\t▏" << left << setw(30) << setfill(separator) << row2[0] 
					<< left << " ▏" << setw(9) << setfill(separator) << row[3]
					<< left << " ▏" << setw(5) << setfill(separator) << "RM" << row[4] << "\t▏"
					<< endl;
				cout << "\t\t----------------------------------------------------------------" << endl;
			}
		}
		cout << "\n\t\t1. Update Order Estimate_Duration" << endl;
		cout << "\n\t\t2. Update Order Status" << endl;
		cout << "\n\t\t3. Print Receipt" << endl;
		cout << "\n\t\t4. Back" << endl;
		cout << "\n\t\t5. Main Menu" << endl;
		cout << "\n\t\tYour Choice:    ";
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:
			EstimatedDuration();
			break;
		case 2:
			UpdateStatus();
			break;
		case 3:
			OrderReport();
			break;
		case 4:
			checkOrder();
			break;
		case 5:
			showStaff();
			break;
		default:
			cout << "\n\t\tPlease choose between 1 and 5. Press Enter To Continue...";
			_getch();
			system("cls");
			checkOrderDetail();
			break;
		}
	}
	else
	{
		cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
	}
}

void EstimatedDuration()
{
	if (Current_Status == "Process")
	{
		UpdateEstimateDuration();
	}
	else if (Current_Status == "Preparing")
	{
		UpdateEstimateDuration();
	}
	else if (Current_Status == "Ready")
	{
		cout << "\n\t\tOrder Have Been Ready....";
		_getch();
		checkOrderDetail();
	}
	else if (Current_Status == "Complete")
	{
		cout << "\n\t\tOrder Have Been Completed....";
		_getch();
		checkOrderDetail();
	}
}

void UpdateEstimateDuration()
{
	int duration;
	cout << "\n\t\tThe duration you want:    ";
	cin >> duration;
	cout << duration << endl;

	string select_query = "UPDATE order_list SET Estimate_Duration='" + to_string(duration) + "' where Order_ID=" + to_string(Current_Order_ID); // update estimate duration
	const char* q = select_query.c_str();
	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		cout << "\n\t\tUpdate Duration: " << to_string(duration) << endl;

		string duration_query = "UPDATE order_list SET Order_Status='Ready' where Estimate_Duration= '0' AND Order_ID=" + to_string(Current_Order_ID); //when estimate duration = 0, the order status need change to ready.
		const char* q = duration_query.c_str();
		qstate = mysql_query(conn, q);

		if (!qstate)
			checkOrderDetail();
	}
	else
		cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
	checkOrderDetail();
}

void UpdateStatus()
{
	cout << "\n\t\t1. Process" << endl;
	cout << "\n\t\t2. Preparing" << endl;
	cout << "\n\t\t3. Ready" << endl;
	cout << "\n\t\t4. Complete" << endl;
	cout << "\n\t\tYour Choice:    ";
	int choice;
	cin >> choice;
	string select_query, status;
	const char* q;

	if (choice == 1)status = "Process";
	else if (choice == 2)status = "Preparing";
	else if (choice == 3)status = "Ready";
	else if (choice == 4)status = "Complete";
	else
	{
		cout << "\n\t\tInvalid Option. Press Enter to Continue...";
		_getch();
		UpdateStatus();
	}

	select_query = "UPDATE order_list SET Order_Status='" + status + "' where Order_ID=" + to_string(Current_Order_ID);
	q = select_query.c_str();
	qstate = mysql_query(conn, q);
	if (!qstate)
	{
		string checkstatus_query = "UPDATE order_list SET Estimate_Duration='0' where Order_Status = 'Ready' AND Order_ID=" + to_string(Current_Order_ID); //if order status = ready, estimate duration will change to 0
		q = checkstatus_query.c_str();
		qstate = mysql_query(conn, q);
		if (!qstate)
			checkOrderDetail();
	}

}

void manageMenu()
{
	system("cls");
	int choice;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\t\t\tManage Menu" << endl;
	cout << "\t\t-----------------------------------------------" << endl;

	cout << "\n\t\t1. Add Menu" << endl;
	cout << "\n\t\t2. Delete Menu" << endl;
	cout << "\n\t\t3. View Menu" << endl;
	cout << "\n\t\t4. Back" << endl;
	cout << "\n\t\tYour Choice:    ";
	cin >> choice;
	switch (choice)
	{
	case 1:
		addMenu();
		break;
	case 2:
		deleteMenu();
		break;
	case 3:
		viewMenu();
		break;
	case 4:
		showStaff();
	default:
		cout << "Please choose between 1 - 5. Press Enter To Continue...";
		_getch();
		system("cls");
		manageMenu();
		break;
	}
}

void addMenu()
{
	system("cls");
	string menuID;
	string Name;
	string unitPrice;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\t\t\tAdd Menu" << endl;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\n\t\tMenu_ID   : ";
	cin.ignore(1, '\n');
	getline(cin, menuID);

	string checkmenu_query = "select Menu_ID from menu where Menu_ID='" + menuID + "'";
	const char* q = checkmenu_query.c_str();
	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (mysql_num_rows(res) > 0)
		{
			char again = ' ';
			cout << "\n\t\tMenuID already exits. Enter Again(Y/N): ";
			cin >> again;
			while ((again != 'Y') && (again != 'y') && (again != 'N') && (again != 'n'))
			{
				cout << "\n\t\tInvalid choice. Please Enter Again:";
				cin >> again;
			}
			if (again == 'Y' || again == 'y')
				addMenu();
			if (again == 'N' || again == 'n')
				manageMenu();
		}
		else
		{
			cout << "\n\n\t\tName      : ";
			getline(cin, Name);
			cout << "\n\n\t\tUnit_Price: RM ";
			getline(cin, unitPrice);

			string insert_query = "insert into menu(Menu_ID, Name, Unit_Price) values('" + menuID + "','" + Name + "','" + unitPrice + "')";
			const char* q = insert_query.c_str();
			qstate = mysql_query(conn, q);

			if (!qstate)
			{
				cout << "\n\n\t\tMenu Successfully insert" << endl;
				char again = ' ';
				cout << "\n\n\t\tDo you want to do add another menu(Y/N): ";
				cin >> again;
				while ((again != 'Y') && (again != 'y') && (again != 'N') && (again != 'n')) {
					cout << "\n\t\tInvalid choice. Please enter again:";
					cin >> again;
				}
				if (again == 'Y' || again == 'y')
					addMenu();
				if (again == 'N' || again == 'n')
					manageMenu();
			}
		}
	}
}

void deleteMenu()
{
	system("cls");
	string menuID2;
	char choice = ' ';

	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\t\t\t\tDelete Menu" << endl;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\n\t\tPlease insert Menu_ID..." << endl;
	cout << "\n\t\tMenu_ID: ";
	cin.ignore(1, '\n');
	getline(cin, menuID2);

	string searchMenu_query = "select Menu_ID, Name, Unit_Price from menu where Menu_ID= '" + menuID2 + "'";
	const char* cu = searchMenu_query.c_str();
	qstate = mysql_query(conn, cu);


	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (row = mysql_fetch_row(res))
		{
			cout << endl;
			cout << "\t\t-----------------------------------------------" << endl;
			cout << "\t\t\tBelow are the Menu Details" << endl;
			cout << "\t\t-----------------------------------------------" << endl;
			cout << "\n\t\tMenuID    : " << row[0] << endl;
			cout << "\n\t\tName      : " << row[1] << endl;
			cout << "\n\t\tUnit Price: RM " << row[2] << endl;
			do {
				cout << "\n\t\tDelete this menu? (Y/N): ";
				cin >> choice;
				if (choice == 'Y' || choice == 'y')
				{
					string deleteMenu_query = "delete from menu where Menu_ID= '" + menuID2 + "'";
					const char* cu = deleteMenu_query.c_str();
					qstate = mysql_query(conn, cu);

					if (!qstate)
					{
						cout << "\n\t\tDelete Successfully!!" << endl;
						char again = ' ';
						cout << "\n\t\tDelete another menu?(Y/N): ";
						cin >> again;
						while ((again != 'Y') && (again != 'y') && (again != 'N') && (again != 'n')) {
							cout << "\n\t\tInvalid choice. Please enter again:";
							cin >> again;
						}
						if (again == 'Y' || again == 'y')
							deleteMenu();
						if (again == 'N' || again == 'n')
							manageMenu();
					}
					else
						cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
				}
				else if (choice == 'N' || choice == 'n')
				{
					manageMenu();
				}
				else
				{
					cout << "\n\n\t\tInvalid option! Please insert again" << endl;

				}
			} while (choice != 'Y' || choice != 'y' || choice != 'N' || choice != 'n');
		}
		else
		{
			cout << "\n\t\tInvalid MenuID. Insert Again (Y/N): ";
			cin >> choice;
			while ((choice != 'Y') && (choice != 'y') && (choice != 'N') && (choice != 'n'))
			{
				cout << "\n\t\tInvalid choice. Please enter again:";
				cin >> choice;
			}
			if (choice == 'Y' || choice == 'y')
				deleteMenu();
			if (choice == 'N' || choice == 'n')
				manageMenu();
		}
	}

	else
	{
		cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
	}

}

void viewMenu()
{
	system("cls");
	string menuID;
	const char separator = ' ';
	cout << "\t\t\t-----------------------------------------------" << endl;
	cout << "\t\t\t▏               View Menu                     ▏" << endl;
	cout << "\t\t\t-----------------------------------------------" << endl;

	qstate = mysql_query(conn, "select Menu_ID, Name, Unit_Price from menu");

	if (!qstate)
	{
		cout << "\t\t-------------------------------------------------------------------------" << endl;
		cout << "\t\t▏ " << left << setw(11) << setfill(separator) << "MenuID" << "▏"
			<< left << setw(30) << setfill(separator) << "Name"
			<< left << setw(7) << "  ▏ " << setfill(separator) << "Unit Price" << "\t\t ▏" << endl;
		cout << "\t\t-------------------------------------------------------------------------" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << "\t\t▏ " << left << setw(11) << setfill(separator) << row[0] << "▏"
				<< left << setw(31) << setfill(separator) << row[1]
				<< left << setw(7) << " ▏ " << setfill(separator) << "RM " << row[2] << "\t\t ▏" << endl;
			cout << "\t\t-------------------------------------------------------------------------" << endl;
		}
	}
	else
	{
		cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
	}
	cout << "\n\t\tPlease press enter to go back Manage Menu.....";
	_getch();
	manageMenu();
}

void OrderReport()
{
	const char separator = ' ';
	string status_query = "select * from order_list where Order_Status='Complete' and Order_ID=" + to_string(Current_Order_ID); //print only order_status=complete
	const char* q = status_query.c_str();
	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (mysql_num_rows(res) > 0)
		{
			string order_query = "select * from order_list where Order_ID=" + to_string(Current_Order_ID);
			const char* q = order_query.c_str();
			qstate = mysql_query(conn, q);

			if (!qstate)
			{
				res = mysql_store_result(conn);
				if (row = mysql_fetch_row(res))
				{
					cout << endl;
					cout << "\n\t\t\t     LAY's RESTAURANT" << endl;
					cout << "\n\t\t\t     Tel: 03-32907341" << endl;
					cout << "\t\t==========================================" << endl;
					cout << "\t\t\t        Order Report" << endl;
					cout << "\t\t==========================================" << endl;
					cout << "\n\t\tOrder DateTime   : " << "    " << row[2] << endl;
					cout << "\n\t\tPayment DateTime : " << "    " << row[3] << endl;
					cout << "\n\t\tOrderID          : " << "\t\t     " << row[0] << endl;
					cout << "\n\t\tCustomerID       : " << "\t\t      " << ID << endl;

					cout << "\n\t\tOrder Summary" << endl;
					cout << endl;

					string receiptdetail_query = "select * from orderlistdetail where Order_ID=" + to_string(Current_Order_ID);
					const char* q = receiptdetail_query.c_str();
					qstate = mysql_query(conn, q);
					if (!qstate)
					{
						res = mysql_store_result(conn);
						while (row = mysql_fetch_row(res))
						{
							string menu_id = row[2];
							string select_query = "select Name from menu where Menu_ID='" + menu_id + "'";
							const char* q = select_query.c_str();
							mysql_query(conn, q);
							res2 = mysql_store_result(conn);
							row2 = mysql_fetch_row(res2);

							cout << "\t\t" << left << setw(2) << setfill(separator) << row[3]
								<< left << setw(30) << setfill(separator) << row2[0] //take from menu table 
								<< left << setw(3) << setfill(separator) << "RM " << row[4]
								<< endl;
						}

						string totalprice_query = "select SUM(Total_Price) from orderlistdetail where Order_ID =" + to_string(Current_Order_ID); // calculation total amount for this order 
						const char* cu = totalprice_query.c_str();
						qstate = mysql_query(conn, cu);

						if (!qstate)
						{
							res = mysql_store_result(conn);
							while (row = mysql_fetch_row(res))
							{
								cout << "\t\t------------------------------------------" << endl;
								cout << "\t\tTotal Price: " << "\t\t\tRM " << row[0] << endl; 
								cout << "\t\t------------------------------------------" << endl;
							}
						}
						cout << "\n\t\tPress Any Key to Continue....";
						_getch();
						checkOrderDetail();

					}
				}
			}
		}
		else
		{
			cout << "\n\t\tOrder Havent Completed. Press Any Key To Continue";
			_getch();
			checkOrderDetail();
		}
	}
}

void ReportType()
{
	system("cls");
	int choice;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\t\t\tSales Report" << endl;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\n\t\t1. Sales Statistics (Menu)" << endl;
	cout << "\n\t\t2. Daily Sales" << endl;
	cout << "\n\t\t3. Monthly Sales" << endl;
	cout << "\n\t\t4. Back" << endl;
	cout << "\n\t\tYour choice: ";
	cin >> choice;
	switch (choice)
	{
	case 1:
		displayReport();
		break;
	case 2:
		dailyReport();
		break;
	case 3:
		monthlyReport();
		break;
	case 4:
		showStaff;
		break;
	default:
		cout << "\n\t\tInvalid choice. Please enter again....";
		_getch();
		system("cls");
		ReportType();
		break;
	}
}

void displayReport()
{
	system("cls");
	const char separator = ' ';
	cout << "\n\t\t                                    Sales Statictisc(Menu)                              " << endl;
	cout << endl;
	//using join show sales for each of the menu
	qstate = mysql_query(conn, "select Menu_ID, Name, Unit_Price, SUM(Quantity), SUM(Total_Price) from orderlistdetail join menu using (Menu_ID)join order_list using (Order_ID)where Payment_DateTime LIKE '2%' group by Menu_ID");

	if (!qstate)
	{
		cout << "\t\t--------------------------------------------------------------------------------------------------------" << endl;
		cout << "\t\t▏ " << left << setw(9) << setfill(separator) << "MenuID" << " ▏ "
			<< left << setw(32) << setfill(separator) << "Name"
			<< left << setw(5) << "▏" << setfill(separator) << "Unit Price" << "\t"
			<< left << setw(2) << " ▏" << setfill(separator) << "Total Quantity"
			<< left << setw(3) << " ▏" << setfill(separator) << "Total Amount" << "\t▏" << endl;
		cout << "\t\t--------------------------------------------------------------------------------------------------------" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << "\t\t▏ " << left << setw(9) << setfill(separator) << row[0] << " ▏ "
				<< left << setw(31) << setfill(separator) << row[1]
				<< left << setw(5) << " ▏" << setfill(separator) << "RM " << row[2] << "\t"
				<< left << setw(10) << " ▏" << setfill(separator) << row[3] << "\t"
				<< left << setw(6) << "  ▏" << setfill(separator) << "RM " << row[4] << "\t\t▏" << endl;
		}
		cout << "\t\t--------------------------------------------------------------------------------------------------------" << endl;

		qstate = mysql_query(conn, "select SUM(Total_Price) from orderlistdetail join order_list using (Order_ID)where Payment_DateTime LIKE '2%'"); //calculate total amount for this sales statistics

		if (!qstate)
		{
			res = mysql_store_result(conn);
			if (row = mysql_fetch_row(res))
			{
				cout << "\n\t\tTotal Profit: RM " << row[0] << endl;
			}
			cout << "\n\t\tPress Any Key to Continue.......";
			_getch();
			ReportType();
		}


	}
}

void dailyReport()
{
	system("cls");
	string Day;
	const char separator = ' ';
	cout << endl;
	cout << "\n\t\tEnter the Day (YYYY-MM-DD): ";
	cin >> Day;

	string checkdate_query = "select Payment_DateTime from order_list where Payment_DateTime LIKE '" + Day + "%'"; //Check if a specific date exists
	const char* q = checkdate_query.c_str();
	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (mysql_num_rows(res) > 0)
		{
			cout << "\n\t\t                                   Daily Sales Statictisc                              " << endl;

			string date_query = "select Menu_ID, Name, Unit_Price, SUM(Quantity), SUM(Total_Price) from orderlistdetail join menu using (Menu_ID)join order_list using (Order_ID)where Payment_DateTime LIKE '" + Day + "%' group by Menu_ID";//Display sales statistics for a particular date
			const char* q = date_query.c_str();
			qstate = mysql_query(conn, q);

			if (!qstate)
			{
				cout << "\t\t-------------------------------------------------------------------------------------------------------" << endl;
				cout << "\t\t▏ " << left << setw(9) << setfill(separator) << "MenuID" << " ▏ "
					<< left << setw(32) << setfill(separator) << "Name"
					<< left << setw(5) << "▏" << setfill(separator) << "Unit Price" << "\t"
					<< left << setw(2) << " ▏" << setfill(separator) << "Total Quantity"
					<< left << setw(3) << " ▏" << setfill(separator) << "Total Amount" << "\t▏" << endl;
				cout << "\t\t-------------------------------------------------------------------------------------------------------" << endl;

				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
				{
					cout << "\t\t▏ " << left << setw(9) << setfill(separator) << row[0] << " ▏ "
						<< left << setw(31) << setfill(separator) << row[1]
						<< left << setw(5) << " ▏" << setfill(separator) << "RM " << row[2] << "\t"
						<< left << setw(10) << " ▏" << setfill(separator) << row[3] << "\t"
						<< left << setw(6) << "  ▏" << setfill(separator) << "RM " << row[4] << "\t\t▏" << endl;
				}
				cout << "\t\t-------------------------------------------------------------------------------------------------------" << endl;

				string total_query = "select SUM(Total_Price) from orderlistdetail join order_list using (Order_ID)where Payment_DateTime LIKE '" + Day + "%'"; //Calculate total amount for this sales statistics
				const char* q = total_query.c_str();
				qstate = mysql_query(conn, q);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (row = mysql_fetch_row(res))
					{
						cout << "\n\t\tTotal Profit: RM " << row[0] << endl;
					}
					cout << "\n\t\tPress Any Key to Continue.......";
					_getch();
					ReportType();
				}
			}
		}
		else
		{
			cout << "\n\t\tNot Report On this Day Or Invalid Day. Press Any Key to Insert Again...";
			_getch();
			dailyReport();
		}
	}

}

void monthlyReport()
{
	system("cls");
	string Month;
	const char separator = ' ';
	cout << endl;
	cout << "\n\t\tEnter the Month (YYYY-MM): ";
	cin >> Month;


	string checkdate_query = "select Payment_DateTime from order_list where Payment_DateTime LIKE '" + Month + "%'"; //Check if a specific month exists
	const char* q = checkdate_query.c_str();
	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (mysql_num_rows(res) > 0)
		{
			cout << "\n\t\t                                  Monthly Sales Statictisc                              " << endl;

			string month_query = "select Menu_ID, Name, Unit_Price, SUM(Quantity), SUM(Total_Price) from orderlistdetail join menu using (Menu_ID)join order_list using (Order_ID)where Payment_DateTime LIKE '" + Month + "%' group by Menu_ID";//Display sales statistics for a particular month
			const char* q = month_query.c_str();
			qstate = mysql_query(conn, q);

			if (!qstate)
			{
				cout << "\t\t--------------------------------------------------------------------------------------------------------" << endl;
				cout << endl;
				cout << "\t\t▏ " << left << setw(9) << setfill(separator) << "MenuID" << " ▏ "
					<< left << setw(32) << setfill(separator) << "Name"
					<< left << setw(5) << "▏" << setfill(separator) << "Unit Price" << "\t"
					<< left << setw(2) << " ▏" << setfill(separator) << "Total Quantity"
					<< left << setw(3) << " ▏" << setfill(separator) << "Total Amount" << "\t▏" << endl;
				cout << "\t\t--------------------------------------------------------------------------------------------------------" << endl;
				cout << endl;
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
				{
					cout << "\t\t▏ " << left << setw(9) << setfill(separator) << row[0] << " ▏ "
						<< left << setw(31) << setfill(separator) << row[1]
						<< left << setw(5) << " ▏" << setfill(separator) << "RM " << row[2] << "\t"
						<< left << setw(10) << " ▏" << setfill(separator) << row[3] << "\t"
						<< left << setw(6) << "  ▏" << setfill(separator) << "RM " << row[4] << "\t\t▏" << endl;
				}
				cout << "\t\t--------------------------------------------------------------------------------------------------------" << endl;

				string totalmonthly_query = "select SUM(Total_Price) from orderlistdetail join order_list using (Order_ID)where Payment_DateTime LIKE '" + Month + "%'";//Calculate total amount for this sales statistics
				const char* q = totalmonthly_query.c_str();
				qstate = mysql_query(conn, q);

				if (!qstate)
				{
					res = mysql_store_result(conn);
					if (row = mysql_fetch_row(res))
					{
						cout << "\n\n\t\tTotal Profit: RM " << row[0] << endl;
					}
					cout << "\n\t\tPress Any Key to Continue.......";
					_getch();
					ReportType();
				}
			}
		}
		else
		{
			cout << "\n\t\tNot Report On this Month Or Invalid Month. Press Any Key to Insert Again...";
			_getch();
			monthlyReport();
		}

	}
}

void interfaceCustomer()
{
	system("cls");
	int choice;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\t\t\t       Customer Homepage" << endl;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\n\t\t1. Login" << endl;
	cout << "\n\t\t2. Register" << endl;
	cout << "\n\t\t3. Main Menu" << endl;
	cout << "\n\t\tYour Choice:    ";
	cin >> choice;
	switch (choice)
	{
	case 1:
		customerLogin();
		break;
	case 2:
		Register();
		break;
	case 3:
		main();
		break;
	default:
		cout << "\n\t\tInvalid Option. Please press enter to type again...";
		_getch();
		system("cls");
		interfaceCustomer();
		break;
	}
}

void showCustomer()
{
	system("cls");
	string CustomerID;
	int choice;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\t\t\t    Customer's Menu" << endl;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\n\t\tCustomerID: " << ID << endl;
	cout << "\n\t\t1. New Order" << endl;
	cout << "\n\t\t2. View Order" << endl;
	cout << "\n\t\t3. Logout" << endl;
	cout << "\n\t\tYour choice: ";
	cin >> choice;
	switch (choice)
	{
	case 1:
		NewOrder();
		break;
	case 2:
		showOrder();
		break;
	case 3:
		interfaceCustomer();
		break;
	default:
		cout << "\n\t\tInvalid Option. Press Any Key To Continue...";
		_getch();
		system("cls");
		showCustomer();
		break;
	}
}

string currentDateTime() {
	CTime t = GetCurrentTime();
	time_t now = time(0);
	tm* ltm = localtime(&now);
	string mytime = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday) + " " + to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);
	return mytime;
}

void NewOrder()
{
	system("cls");
	const char separator = ' ';
	cout << "\t\t-------------------------------------------------------------------------" << endl;
	cout << "\t\t\t\t                New Order" << endl;
	cout << "\t\t-------------------------------------------------------------------------" << endl;

	qstate = mysql_query(conn, "select Menu_ID, Name, Unit_Price from menu"); //show menu table

	if (!qstate)
	{
		cout << "\t\t-------------------------------------------------------------------------" << endl;
		cout << "\t\t▏ " << left << setw(11) << setfill(separator) << "MenuID" << "▏"
			<< left << setw(30) << setfill(separator) << "Name"
			<< left << setw(7) << "  ▏ " << setfill(separator) << "Unit Price" << "\t\t ▏" << endl;
		cout << "\t\t-------------------------------------------------------------------------" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << "\t\t▏ " << left << setw(11) << setfill(separator) << row[0] << "▏"
				<< left << setw(31) << setfill(separator) << row[1]
				<< left << setw(7) << " ▏ " << setfill(separator) << "RM " << row[2] << "\t\t ▏" << endl;
			cout << "\t\t-------------------------------------------------------------------------" << endl;

		}
	}
	else
	{
		cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
	}
	char choice = ' ';
	string MenuID;
	string Quantity;
	cout << "\n\t\tCustomerID: " << ID << endl;

	char sdate[30];
	strcpy_s(sdate, currentDateTime().c_str());
	string order_query = "insert into order_list values(NULL," + ID + ",'" + sdate + "'," + "'NULL'," + "'Process'," + to_string(0) + ")"; //insert customerid,order_datetime, payment_datetime=NULL, order_status, estimated duration into table orderlist
	const char* q = order_query.c_str();
	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		string check_query = "select LAST_INSERT_ID();"; //read last orderid because order id auto increment when a new order exist
		const char* q = check_query.c_str();
		qstate = mysql_query(conn, q);

		if (!qstate)
		{
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);

			Same_OrderID = stoi(row[0]); //string to integer
			cin.ignore(1, '\n');
			cout << "\n\t\tMenu ID     : ";
			getline(cin, MenuID);

			string checkMenu_query = "select Menu_ID from menu where Menu_ID='" + MenuID + "'";
			const char* q = checkMenu_query.c_str();
			qstate = mysql_query(conn, q);

			if (!qstate)
			{
				res = mysql_store_result(conn);
				if (mysql_num_rows(res) > 0)
				{
					cout << "\n\t\tQuantity    : ";
					getline(cin, Quantity);
					string price_query = "select Unit_Price from menu where Menu_ID='" + MenuID + "'";
					const char* q = price_query.c_str();
					qstate = mysql_query(conn, q);

					if (!qstate)
					{
						res = mysql_store_result(conn);
						row = mysql_fetch_row(res);
						cout << "\n\t\tTotal Price : RM " << stoi(Quantity) * atof(row[0]) << endl; 
						float total_price = stoi(Quantity) * atof(row[0]); //calculate total price = quantity * unit price

						string order_query = "insert into orderlistdetail values(NULL," + to_string(Same_OrderID) + ",'" + MenuID + "'," + Quantity + "," + to_string(total_price) + ")";
						const char* q = order_query.c_str();
						qstate = mysql_query(conn, q);

						if (!qstate)
						{
							cout << "\n\t\tAdd Another Menu(Y/N): ";
							cin >> choice;
							while ((choice != 'Y') && (choice != 'y') && (choice != 'N') && (choice != 'n'))
							{
								cout << "\n\t\tInvalid choice. Please Enter Again(Y/N):";
								cin >> choice;
							}
							if (choice == 'Y' || choice == 'y')
								anotherMenu();
							if (choice == 'N' || choice == 'n')
								cout << "\n\t\t" << currentDateTime() << endl; //print current local datetime
							cout << "\n\t\tSuccessfully make an order. Please wait restaurant accepted" << endl;
							char again = ' ';
							cout << "\n\t\tAdd New Order(Y/N):";
							cin >> again;
							while ((again != 'Y') && (again != 'y') && (again != 'N') && (again != 'n')) {
								cout << "\n\t\tInvalid choice. Please Enter Again(Y/N):";
								cin >> again;
							}
							if (again == 'Y' || again == 'y')
								NewOrder();
							if (again == 'N' || again == 'n')
								showCustomer();
						}
						else
							cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
					}
					else
						cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
				}
				cout << "\n\t\tInvalid MenuID. Enter Again? (Y/N): ";
				cin >> choice;
				while ((choice != 'Y') && (choice != 'y') && (choice != 'N') && (choice != 'n'))
				{
					cout << "\n\t\tInvalid choice. Please Enter Again(Y/N):";
					cin >> choice;
				}
				if (choice == 'Y' || choice == 'y')
					NewOrder();
				if (choice == 'N' || choice == 'n')
					showCustomer();
			}
			else
				cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
		}
		else
			cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
	}
	else
		cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
}

void anotherMenu()
{
	char again = ' ';
	char choice = ' ';
	string MenuID;
	string Quantity;

	cin.ignore(1, '\n');
	cout << "\n\t\tMenu ID     : ";
	getline(cin, MenuID);

	string checkMenu_query = "select Menu_ID from menu where Menu_ID='" + MenuID + "'";
	const char* q = checkMenu_query.c_str();
	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (mysql_num_rows(res) > 0)
		{
			cout << "\n\t\tQuantity    : ";
			getline(cin, Quantity);
			string price_query = "select Unit_Price from menu where Menu_ID='" + MenuID + "'";
			const char* q = price_query.c_str();
			qstate = mysql_query(conn, q);

			if (!qstate)
			{
				res = mysql_store_result(conn);
				row = mysql_fetch_row(res);
				cout << "\n\t\tTotal Price : RM " << stoi(Quantity) * atof(row[0]) << endl;
				float total_price = stoi(Quantity) * atof(row[0]);

				string order_query = "insert into orderlistdetail values(NULL," + to_string(Same_OrderID) + ",'" + MenuID + "'," + Quantity + "," + to_string(total_price) + ")";
				const char* q = order_query.c_str();
				qstate = mysql_query(conn, q);

				if (!qstate)
				{
					cout << "\n\t\tSuccefully add menu" << endl;
					cout << "\n\t\tAdd Another Menu (Y/N): ";
					cin >> choice;
					while ((choice != 'Y') && (choice != 'y') && (choice != 'N') && (choice != 'n'))
					{
						cout << "\n\t\tInvalid choice. Please enter again:";
						cin >> choice;
					}
					if (choice == 'Y' || choice == 'y')
						anotherMenu();
					if (choice == 'N' || choice == 'n')
						cout << "\n\t\t" << currentDateTime() << endl;
					cout << "\n\t\tSuccessfully make an order. Please wait restaurant accepted" << endl;

					cout << "\n\t\tAdd New Order(Y/N):      ";
					cin >> again;
					while ((again != 'Y') && (again != 'y') && (again != 'N') && (again != 'n')) {
						cout << "\n\t\tInvalid choice. Please enter again:";
						cin >> again;
					}
					if (again == 'Y' || again == 'y')
						NewOrder();
					if (again == 'N' || again == 'n')
						showCustomer();
				}
				else
					cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
			}
			else
				cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
		}
		else
		{
			cout << "\n\t\tInvalid MenuID. Insert Again (Y/N): ";
			cin >> choice;
			while ((choice != 'Y') && (choice != 'y') && (choice != 'N') && (choice != 'n'))
			{
				cout << "\n\t\tInvalid choice. Please enter again:";
				cin >> choice;
			}
			if (choice == 'Y' || choice == 'y')
				anotherMenu();
			if (choice == 'N' || choice == 'n')
				cout << "\n\t\t" << currentDateTime() << endl;
		}
	}
	else
		cout << "\n\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
}

void showOrder()
{
	system("cls");
	const char separator = ' ';
	cout << "\t\t-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "\t\t\t\t                                  View Order" << endl;
	cout << "\t\t-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "\n\t\tCustomerID: " << ID << endl;
	cout << endl;

	string viewOrder_query = "select * from order_list where Customer_ID=" + ID + " ORDER BY Order_Status ASC"; //view all order for customer
	const char* cu = viewOrder_query.c_str();
	qstate = mysql_query(conn, cu);


	if (!qstate)
	{
		cout << "\t\t-----------------------------------------------------------------------------------------------------------------------" << endl;
		cout << "\t\t▏" << left << setw(7) << setfill(separator) << "Order ID" << " ▏"
			<< left << " ▏" << setw(1) << setfill(separator) << "Customer ID" << " ▏"
			<< left << " ▏" << setw(19) << setfill(separator) << "Order_DateTime" << " ▏"
			<< left << " ▏" << setw(19) << setfill(separator) << "Payment_DateTime" << " ▏"
			<< left << " ▏" << setw(8) << setfill(separator) << "Order_Status" << " ▏"
			<< left << " ▏" << setw(8) << setfill(separator) << "Estimate_Duration" << " ▏"
			<< endl;
		cout << "\t\t-----------------------------------------------------------------------------------------------------------------------" << endl;

		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << "\t\t▏" << left << setw(8) << setfill(separator) << row[0] << " ▏"
				<< left << " ▏" << setw(11) << setfill(separator) << row[1] << " ▏" 
				<< left << " ▏" << setw(9) << setfill(separator) << row[2] << " ▏"
				<< left << " ▏" << setw(10) << setfill(separator) << row[3] << " ▏"
				<< left << " ▏" << setw(12) << setfill(separator) << row[4] << " ▏"
				<< left << " ▏" << setw(17) << setfill(separator) << row[5] << " ▏"
				<< endl;

			cout << "\t\t-----------------------------------------------------------------------------------------------------------------------" << endl;
		}
		int choice;
		cout << "\n\t\t1. View Order Detail & Payment" << endl;
		cout << "\n\t\t2. Receipt" << endl;
		cout << "\n\t\t3. Back" << endl;
		cout << "\n\t\tYour Choice: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			viewOrder();
			break;
		case 2:
			checkReceiptOrder();
			break;
		case 3:
			showCustomer();
			break;
		default:
			cout << "\n\t\tPlease choose between 1 and 3. Press Enter To Continue...";
			_getch();
			system("cls");
			showOrder();
			break;
		}
	}
}

void viewOrder()
{
	string OrderStatus;
	int OrderID;
	cout << "\t\t----------------------------------------------------------------------------------------------" << endl;
	cout << "\n\t\tOrderID  : ";
	cin >> OrderID;

	string order_query = "select * from order_list where Order_ID  = " + to_string(OrderID);
	const char* q = order_query.c_str();
	qstate = mysql_query(conn, q);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (mysql_num_rows(res) > 0)
		{
			Current_Order_ID = OrderID;

			string status_query = "select * from order_list where Order_ID  = " + to_string(OrderID);
			const char* q = status_query.c_str();
			qstate = mysql_query(conn, q);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				row = mysql_fetch_row(res);
				Current_Status = row[4];
				viewOrderDetail();
			}
			
		}
		else
		{
			cout << "\n\t\tNo OrderID " << endl;
			char choice = ' ';
			cout << "\n\t\tContinue or back(Y/N): ";
			cin >> choice;
			while ((choice != 'Y') && (choice != 'y') && (choice != 'N') && (choice != 'n'))
			{
				cout << "\n\t\tInvalid choice. Please enter again:";
				cin >> choice;
			}
			if (choice == 'Y' || choice == 'y')
				viewOrder();
			if (choice == 'N' || choice == 'n')
				showCustomer();
		}

	}
	
}

void viewOrderDetail()
{
	system("cls");
	const char separator = ' ';
	cout << "\n\t\tOrderID: " << Current_Order_ID << endl;
	cout << endl;
	string order_query = "select * from orderlistdetail where Order_ID=" + to_string(Current_Order_ID); 
	const char* q = order_query.c_str();
	qstate = mysql_query(conn, q);
	if (!qstate)
	{
		cout << "\t\t----------------------------------------------------------------" << endl;

		cout << "\t\t▏" << left << setw(30) << setfill(separator) << "Menu"
			<< left << " ▏" << setw(9) << setfill(separator) << "Quantity"
			<< left << " ▏" << setw(4) << setfill(separator) << "Total_Price" << "\t▏"
			<< endl;

		cout << "\t\t----------------------------------------------------------------" << endl;
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			string menu_id = row[2];
			string select_query = "select Name from menu where Menu_ID='" + menu_id + "'";
			const char* q = select_query.c_str();
			mysql_query(conn, q);
			res2 = mysql_store_result(conn);
			row2 = mysql_fetch_row(res2);
			{
				cout << "\t\t▏" << left << setw(30) << setfill(separator) << row2[0]
					<< left << " ▏" << setw(9) << setfill(separator) << row[3]
					<< left << " ▏" << setw(5) << setfill(separator) << "RM" << row[4] << "\t▏"
					<< endl;
				cout << "\t\t----------------------------------------------------------------" << endl;
			}
		}
		printStatus();
	}
}

void printStatus()
{
	cout << "\n\t\tOrder Status: " << Current_Status << endl; //print order status
	if (Current_Status == "Process")
	{
		cout << "\n\t\tOrder still in Process. Thank you for ordering.......";
		_getch();
		showOrder();
	}
	else if (Current_Status == "Preparing")
	{
		cout << "\n\t\tOrder still in Preparing. Thank you for ordering.......";
		_getch();
		showOrder();
	}
	else if (Current_Status == "Ready")
	{
		string date_query = "select Payment_DateTime from order_list where Payment_DateTime = ('NULL') and Order_ID=" + to_string(Current_Order_ID); //check whether payment have done or not
		const char* q = date_query.c_str();
		qstate = mysql_query(conn, q);

		if (!qstate)
		{
			res = mysql_store_result(conn);
			if (mysql_num_rows(res) > 0)
			{
				char choice = ' ';
				string sdate = currentDateTime().c_str();
				string order_query = "UPDATE order_list SET Payment_DateTime='" + sdate + "' where Order_ID=" + to_string(Current_Order_ID);//if order_status = ready, customer can make payment.
				const char* q = order_query.c_str();
				qstate = mysql_query(conn, q);

				if (!qstate)
				{
					cout << "\n\t\tDo You Want Make Payment for This Order (Y/N): ";
					cin >> choice;
					while ((choice != 'Y') && (choice != 'y') && (choice != 'N') && (choice != 'n'))
					{
						cout << "\n\t\tInvalid choice. Please enter again:";
						cin >> choice;
					}
					if (choice == 'Y' || choice == 'y')
					{
						cout << "\n\t\tPayment DateTime: " << currentDateTime() << endl;
						cout << "\n\t\tSuccessfully completed the order!!!";
						_getch();
						showOrder();
					}
					if (choice == 'N' || choice == 'n')
						showOrder();
				}
			}
			else
			{
				cout << "\n\t\tPayment Have Been Done. Please Wait for Staff Approval.....";
				_getch();
				showOrder();
			}
		}
	}
	else if (Current_Status == "Complete")
	{
		cout << "\n\t\tOrder Was Completed. Print Receipt Was Available...";
		_getch();
		showOrder();
	}
}

void checkReceiptOrder()
{
	string OrderStatus;
	int OrderID;
	cout << "\t\t----------------------------------------------------------------------------------------------" << endl;
	cout << "\n\t\tOrderID  : ";
	cin >> OrderID;

	string order_query = "select * from order_list where Order_ID  = " + to_string(OrderID);
	const char* q = order_query.c_str();
	qstate = mysql_query(conn, q);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (mysql_num_rows(res) > 0)
		{
			Current_Order_ID = OrderID;

			string status_query = "select * from order_list where Order_ID  = " + to_string(OrderID);
			const char* q = status_query.c_str();
			qstate = mysql_query(conn, q);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				row = mysql_fetch_row(res);
				Current_Status = row[4];
				PrintReceipt();
			}

		}
		else
		{
			cout << "\n\t\tNo OrderID " << endl;
			char choice = ' ';
			cout << "\n\t\tContinue or back(Y/N): ";
			cin >> choice;
			while ((choice != 'Y') && (choice != 'y') && (choice != 'N') && (choice != 'n'))
			{
				cout << "\n\t\tInvalid choice. Please enter again:";
				cin >> choice;
			}
			if (choice == 'Y' || choice == 'y')
				viewOrder();
			if (choice == 'N' || choice == 'n')
				showCustomer();
		}

	}

}

void PrintReceipt()
{
	const char separator = ' ';
	string status_query = "select * from order_list where Order_Status='Complete' and Order_ID=" + to_string(Current_Order_ID);
	const char* q = status_query.c_str();
	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (mysql_num_rows(res) > 0)
		{
			string order_query = "select * from order_list where Order_ID=" + to_string(Current_Order_ID);
			const char* q = order_query.c_str();
			qstate = mysql_query(conn, q);

			if (!qstate)
			{
				res = mysql_store_result(conn);
				if (row = mysql_fetch_row(res))
				{
					cout << endl;
					cout << "\n\t\t\t     LAY's RESTAURANT" << endl;
					cout << "\n\t\t\t     Tel: 03-32907341" << endl;
					cout << "\t\t==========================================" << endl;
					cout << "\t\t\t        RECEIPT" << endl;
					cout << "\t\t==========================================" << endl;
					cout << "\n\t\tOrder DateTime   : " << "    " << row[2] << endl;
					cout << "\n\t\tPayment DateTime : " << "    " << row[3] << endl;
					cout << "\n\t\tOrderID          : " << "\t\t     " << row[0] << endl;
					cout << "\n\t\tCustomerID       : " << "\t\t      " << ID << endl;

					cout << "\n\t\tOrder Summary" << endl;
					cout << endl;

					string receiptdetail_query = "select * from orderlistdetail where Order_ID=" + to_string(Current_Order_ID);
					const char* q = receiptdetail_query.c_str();
					qstate = mysql_query(conn, q);
					if (!qstate)
					{
						res = mysql_store_result(conn);
						while (row = mysql_fetch_row(res))
						{
							string menu_id = row[2];
							string select_query = "select Name from menu where Menu_ID='" + menu_id + "'";
							const char* q = select_query.c_str();
							mysql_query(conn, q);
							res2 = mysql_store_result(conn);
							row2 = mysql_fetch_row(res2);

							cout << "\t\t" << left << setw(2) << setfill(separator) << row[3]
								<< left << setw(30) << setfill(separator) << row2[0] //这边取menu里面的name
								<< left << setw(3) << setfill(separator) << "RM " << row[4]
								<< endl;
						}

						string totalprice_query = "select SUM(Total_Price) from orderlistdetail where Order_ID =" + to_string(Current_Order_ID);
						const char* cu = totalprice_query.c_str();
						qstate = mysql_query(conn, cu);

						if (!qstate)
						{
							res = mysql_store_result(conn);
							while (row = mysql_fetch_row(res))
							{
								cout << "\t\t------------------------------------------" << endl;
								cout << "\t\tTotal Price: " << "\t\t\tRM " << row[0] << endl;
								cout << "\t\t------------------------------------------" << endl;
							}
							cout << "\n\t\t\t\tTHANK YOU" << endl;
							cout << "\t\t==========================================" << endl;
						}
						cout << "\n\t\tEnter any key back to Main Menu: ";
						_getch();
						showOrder();

					}
				}
			}
		}
		else
		{
			cout << "\n\t\tOrder Havent Completed. Press Any Key To Continue";
			_getch();
			showOrder();
		}
	}
}


/*---------------------main--------------------------*/
int main()
{
	ConnectionFunction();
	system("cls");
	int choice;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\t\t\tWelcome to Lay's Restaurant!" << endl;
	cout << "\t\t-----------------------------------------------" << endl;
	cout << "\n\t\t1. Staff" << endl;
	cout << endl;
	cout << "\t\t2. Customer" << endl;
	cout << endl;
	cout << "\t\t3. Exit" << endl;
	cout << endl;
	cout << "\t\tYour choice:      ";
	cin >> choice;
	switch (choice)
	{
	case 1:
		staffLogin();
		break;
	case 2:
		interfaceCustomer();
		break;
	case 3:
		exit(0);
		break;
	default:
		cout << "\n\t\tInvalid Option. Please press enter to type again...";
		_getch();
		system("cls");
		main();
		break;
	}
	return 0;
}

