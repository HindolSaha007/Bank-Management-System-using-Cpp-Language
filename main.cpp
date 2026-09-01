#include<iostream>
#include<string.h>
#include<fstream>

using namespace std;

//STRUCTURE OF THE ACCOUNT 

class Account
{
	protected:
		int acc_no;
		char name_of_the_holder[20];
		int PIN;
		char date_of_opening[11];
		int age;
		float total_balance;
		
	public:
		
		//CREATING OR ADDING AN ACCOUNT
		void create_acc()
		{
			int temp_acc_no, temp_age, v, temp_amt;
			
			ofstream bank_acc("Bank_Acc.bin", ios::binary | ios::app);
			
			cout<<"\n\nCreating a new Account--->\n";
			cout<<"Create an Account number: ";
			cin>>temp_acc_no;
			cin.ignore();
			v = is_acc_avl(temp_acc_no);
			if(v==1)
			{
				cout<<"\nAccount number already exists";
				return;
			}
			else
			{
				acc_no = temp_acc_no;
			}
			
			cout<<"Enter the name of the Account holder: ";
			cin.getline(name_of_the_holder, 20);
			
			cout<<"Enter "<<name_of_the_holder<<"'s age: ";
			cin>>temp_age;
			cin.ignore();
			if(temp_age < 18)
			{
				cout<<"\nYou have to be atleast 18 or above to open an Account";
				return;
			}
			else
			{
				age = temp_age;
			}
			
			cout<<"Enter today's date (e.g. xx-xx-xxxx): ";
			cin.getline(date_of_opening, 11);
			
			cout<<"Set a 4-digit PIN: (Do not share this with anyone) ";
			cin>>PIN;
			cin.ignore();
			
			cout<<"Deposit amount to open account (Minimum amount is Rs 1000): ";
			cin>>temp_amt;
			cin.ignore();
			if (temp_amt < 1000)
			{
				cout<<"\nMinimum amount should be 1000 or higher";
				return;
			}
			else
			{
				total_balance = temp_amt;
			}
			
			bank_acc.write((char*)this, sizeof(Account));
			cout<<"\nAccount created successfully";
			bank_acc.close();
		}
		
		//CHECK AVAILABILITY OF AN ACCOUNT
		int is_acc_avl(int x)
		{
			ifstream bank_acc("Bank_Acc.bin", ios::binary);
			
			if (bank_acc.is_open())
			{
				while(bank_acc.read((char*)this, sizeof(Account)))
				{
					if (acc_no == x)
					{
						return 1;    //acc is present
					}
				}
			}
			bank_acc.close();
			return 0;
		}
		
		//SEARCH FOR AN ACCOUNT USING ACC NUMBER
		void search_acc()
		{
			ifstream src("Bank_Acc.bin", ios::binary);
			int src_acc, v;
			
			if(src == NULL)
			{
				cout<<"Oops it seems there's no records available";
			}
			
			else
			{
				cout<<"Enter the Account number to search: ";
				cin>>src_acc;
				v = is_acc_avl(src_acc);
			
				if (v==1)
				{
					while(src.read((char*)this, sizeof(Account)))
					{
						if(src_acc == acc_no)
						{
							cout<<"\n    ACCOUNTHOLDER DETAILS";
							cout<<"\n_________________________________\n";
							cout<<"Name of the account holder: "<<name_of_the_holder<<"\n";
							cout<<"Age of the account holder: "<<age<<"\n";
							cout<<"Date of opening the Account: "<<date_of_opening<<"\n";
							cout<<"Available Balance: "<<total_balance<<"\n";
							cout<<"------------------------------------";
						}
					}
				}
				else
				{
					cout<<"Account doesn't exists.\n";
				}
			}
		}
		
		//DEPOSITING AMOUNT
		void deposit_amount()
		{
			int d_acc_no, v;
			float deposit_amt;
			char ch[2];
			
			ifstream read("Bank_Acc.bin", ios::binary);
			
			if(read == NULL)
			{
				cout<<"Oops it seems there's no record available\n";
			}
			
			else
			{
				ofstream write("Temp_Acc.bin", ios::binary);
				
				cout<<"\n\n\t\t___Deposit Dashboard___\n\n";
				cout<<"\t\tEnter the deposit amount: ";
				cin>>deposit_amt;
				cout<<"\t\tEnter the Account number to proceed: ";
				cin>>d_acc_no;
				cin.ignore();
				v = is_acc_avl(d_acc_no);
				
				if(v==1)
				{
					while(read.read((char*)this, sizeof(Account)))
					{
						if (d_acc_no == acc_no)
						{
							cout<<"\t\tName of the Account holder: "<<name_of_the_holder;
							cout<<"\n\nContinue your Deposit (Y/N): ";
							cin.getline(ch, 2);
							
							if(strcmp(ch, "N")==0)
							{
								cout<<"\nDeposit cancelled\n";
								return;
							}
							
							else if(strcmp(ch, "Y")==0)
							{
								total_balance = total_balance + deposit_amt;
								write.write((char*)this, sizeof(Account));
							}
							
							else
							{
								cout<<"Oops wrong choice\n";
								return;
							}
						}
						
						else
						{
							write.write((char*)this, sizeof(Account));
						}	
					}
					cout<<"\nAmount deposited successfully\n";
				}
				else
				{
					cout<<"Account doesn't exists";
					return;
				}
				write.close();
				read.close();
				
				remove("Bank_Acc.bin");
				rename("Temp_Acc.bin", "Bank_Acc.bin");	
			}	
		}
		
		//WITHDRAWING MONEY
		void withdraw_amount()
		{
			ifstream read("Bank_Acc.bin", ios::binary);
			int wdr_acc_no, v;
			float wdr_amt;
			
			if (read == NULL)
			{
				cout<<"Oops it seems there's no record available\n";
				return;
			}
			
			else
			{
				ofstream write("Temp_Acc.bin", ios::binary);
				
				cout<<"\n\n\t\tWithdraw Dashboard\n\n";
				cout<<"\t\tEnter the account number: ";
				cin>>wdr_acc_no;
				cin.ignore();
				cout<<"\t\tEnter the amount to withdraw: ";
				cin>>wdr_amt;
				cin.ignore();
				v = is_acc_avl(wdr_acc_no);
				
				if (v==1)
				{
					while(read.read((char*)this, sizeof(Account)))
					{
						if (acc_no == wdr_acc_no)
						{
							cout<<"\t\tAccount holder's name: "<<name_of_the_holder;
							
							if(wdr_amt > total_balance)
							{
								cout<<"\n\nInsufficient balance\n";
								cout<<"\nAvailable Balance: "<<total_balance<<"\n";
								return;
							}
							
							else
							{
								total_balance = total_balance - wdr_amt;
								write.write((char*)this, sizeof(Account));
							}
						}
						
						else
						{
							write.write((char*)this, sizeof(Account));
						}
					}
					cout<<"\n\nAmount withdrawn successfully\n";
				}
				
				else
				{
					cout<<"Account doesn't exists\n";
					return;
				}
				read.close();
				write.close();
				
				remove("Bank_Acc.bin");
				rename("Temp_Acc.bin", "Bank_Acc.bin");	
			}
		}
		
		//TRANSFER MONEY FROM ONE ACCOUNT TO OTHER
		void transfer()
		{
			int t_acc_no, r_acc_no, tpin, v1, v2;
			float t_amt;
			
			ifstream read("Bank_Acc.bin", ios::binary);
			
			if(read==NULL)
			{
				cout<<"Oops it seems there's no record available\n";
				return;
			}
			
			else
			{
				cout<<"Enter your account number: ";
				cin>>t_acc_no;
				cout<<"Enter the account number of the receiver: ";
				cin>>r_acc_no;
				if (t_acc_no == r_acc_no)
				{
					cout<<"\nSelf transfer is not possible";
					return;
				}
			
				v1 = is_acc_avl(t_acc_no);
				v2 = is_acc_avl(r_acc_no);
			
				if(v1==1 && v2==1)
				{
					cout<<"Enter amount: ";
					cin>>t_amt;
					cout<<"Enter the 4-digit PIN to continue: ";
					cin>>tpin;
					
					ofstream write ("Temp_Acc.bin", ios::binary);
					
					while(read.read((char*)this, sizeof(Account)))
					{
						if (acc_no == t_acc_no)
						{
							if (PIN == tpin)
							{
								total_balance = total_balance - t_amt;
								write.write((char*)this, sizeof(Account));
							}
							else
							{
								cout<<"\nOops wrong PIN. Try again\n";
								return;
							}
						}
						else
						{
							write.write((char*)this, sizeof(Account));
						}
					}
					cout<<"\n\nTransfer successfull\n";
					
					read.close();
					write.close();
				
					remove ("Bank_Acc.bin");
					rename ("Temp_Acc.bin", "Bank_Acc.bin");	
				}
				
				else
				{
					cout<<"\n\nEither of the account number is incorrect. Check and try again\n";
					return;
				}
				
				ifstream read2 ("Bank_Acc.bin", ios::binary);
				ofstream write2 ("Temp_Acc.bin", ios::binary);
				
				while(read2.read((char*)this, sizeof(Account)))
				{
					if (acc_no == r_acc_no)
					{
						total_balance = total_balance + t_amt;
						write2.write((char*)this, sizeof(Account));
					}
					
					else
					{
						write2.write((char*)this, sizeof(Account));
					}
				}
				
				read2.close();
				write2.close();
				
				remove ("Bank_Acc.bin");
				rename ("Temp_Acc.bin", "Bank_Acc.bin");
			}
		}
		
		
		//CLOSING AN ACCOUNT
		void closing_acc()
		{
			int c_acc_no, pin, v;
			char c[2];
			
			ifstream read ("Bank_Acc.bin", ios::binary);
			
			if(read == NULL)
			{
				cout<<"Oops it seems there's no record available\n";
				return;
			}
			
			else
			{
				ofstream write ("Temp_Acc.bin", ios::binary);
				
				cout<<"Enter your Account number: ";
				cin>>c_acc_no;
				cin.ignore();
				v = is_acc_avl(c_acc_no);
				
				if (v==1)
				{
					cout<<"Are you sure you want to close your Account (Y/N): ";
					cin.getline(c, 2);
					
					if(strcmp(c, "N")==0)
					{
						cout<<"\n\nAccount Deletation exited...\n";
						return;
					}
				
					else if (strcmp(c, "Y")==0)
					{
						cout<<"Enter your 4-digit PIN to continue deleting account: ";
						cin>>pin;
					
						while(read.read((char*)this, sizeof(Account)))
						{
							if (acc_no == c_acc_no)
							{
								if(PIN == pin)
								{
									if (acc_no != c_acc_no)
									{
										write.write((char*)this, sizeof(Account));
									}
								}
								
								else
								{
									cout<<"\n\nOops wrong PIN. Account deletation exited\n";
									return;
								}
							}
						}
					}
					
					else
					{
						cout<<"\n\nWrong choice\n";
						return;
					}
				}

				else
				{
					cout<<"\nAccount doesn't exist";
					return;
				}
				cout<<"\n\nAccount successfully closed\n";
				
				read.close();
				write.close();
				
				remove ("Bank_Acc.bin");
				rename ("Temp_Acc.bin", "Bank_Acc.bin");
			}
			
		}
		
		
};

main()
{
	class Account a;
	int ch;
	while (1)
	{
		cout<<"\n\n\t\t\tBANK MANAGEMENT\n";
		cout<<"\t\t_______________________________\n";
		cout<<"\t\t1. Proceed with Creating Account\n";
		cout<<"\t\t2. Search for any account\n";
		cout<<"\t\t3. Deposit Money\n";
		cout<<"\t\t4. Withdraw Money\n";
		cout<<"\t\t5. Transfer Money\n";
		cout<<"\t\t6. Close account\n";
		cout<<"\nEnter your choice: ";
		cin>>ch;
		switch(ch)
		{
			case 1:
				{
					a.create_acc();
				}
				break;
				
			case 2:
				{
					a.search_acc();
				}
				break;
				
			case 3:
				{
					a.deposit_amount();
				}
				break;
				
			case 4:
				{
					a.withdraw_amount();
				}
				break;
				
			case 5:
				{
					a.transfer();
				}
				break;
				
			case 6:
				{
					a.closing_acc();
				}
				break;
				
			default:
				{
					cout<<"Oops wrong option\n";
				}
				break;
		}
	}
	return 0;
}
