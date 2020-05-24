//******************************
// Project: Transaction History
// Name: Preyansh Kachhia
// Student ID: 20828862
// Professor: Hiren Patel
// Date: December 3, 2019
//******************************

#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////

// Constructor// TASK
//
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,
        unsigned int month_date,  unsigned year_date,
        bool buy_sell_trans,  unsigned int number_shares,
        double trans_amount ) {

	this->symbol = ticker_symbol;
	this->day = day_date;
	this->month = month_date;
	this->year = year_date;

	if(buy_sell_trans == true) {
		this->trans_type = "Buy";
	}
	else{
		this->trans_type = "Sell";
	}

	this->shares = number_shares;
	this->amount = trans_amount;

	this->trans_id = assigned_trans_id;
	assigned_trans_id++;

	this->acb = 0;
	this->acb_per_share = 0;
	this->share_balance = 0;
	this->cgl = 0;

	this->p_next = nullptr;
}

// Destructor
// TASK
//
Transaction::~Transaction (){

}

// TASK
// Overloaded < operator.
//
bool Transaction::operator<( Transaction const &other ) {
	if(this->year < other.get_year()) {
		return true;
	}
	else if(this->year == other.get_year() && this->month < other.get_month()) {
		return true;
	}
	else if(this->year == other.get_year() && this->month == other.get_month() && this->day < other.get_day()) {
		return true;
	}
	else{
		return false;
	}
}

// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values. 
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " " 
    << std::setw(4) << get_symbol() << " " 
    << std::setw(4) << get_day() << " "  
    << std::setw(4) << get_month() << " " 
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) { 
    std::cout << "  Buy  "; 
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " " 
    << std::setw(10) << get_amount() << " " 
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl() 
    << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//

History::History() {
	p_head = nullptr; //set the head of the linked_list as null => so initially there is no nodes
}

// Destructor
// TASK
//

History::~History() {
	Transaction *p_del_head {p_head};

	//scan through the linked list until you reach the last node
	//i.e the pointer p_del_head will go through each node until it = nullptr
	while(p_del_head != nullptr) {
		p_del_head = p_del_head->get_next();
		delete p_head;
		p_head = p_del_head;
	}
}

// TASK
// read_transaction(...): Read the transaction history from file. 
//

void History::read_history() {
	std::string symbol {};
	unsigned int day{0};
	unsigned int month{0};
	unsigned int year{0};
	unsigned int num_shares{0};
	bool trans_type{false};       //True if Buy, False if sell
	double trans_amount{0};

	int count{0};
	ece150::open_file();

	while(ece150::next_trans_entry() != false) {
		symbol =  ece150::get_trans_symbol();
		day = ece150::get_trans_day();
		month = ece150::get_trans_month();
		year = ece150::get_trans_year();
		trans_type = ece150::get_trans_type();
		num_shares = ece150::get_trans_shares();
		trans_amount = ece150::get_trans_amount();

		Transaction* p_new_trans{new Transaction(symbol, day, month, year,
				trans_type, num_shares, trans_amount)};

		insert(p_new_trans);
		count++;
	}
	ece150::close_file();
}

// insert(...): Insert transaction into linked list.
//
void History::insert(Transaction *p_new_trans) {

	//Insert most recent transaction at the end of linked list
	if(p_head == nullptr) {
		p_head = p_new_trans;
	}
	else {
		Transaction * p_last_trans{p_head};

		while(p_last_trans->get_next() != nullptr) {
			p_last_trans = p_last_trans->get_next();
		}

		p_last_trans->set_next(p_new_trans);
	}
}

// TASK
// sort_by_date(): Sort the linked list by trade date.
//

void History::sort_by_date() {
	if(p_head != nullptr) {
		Transaction * p_previous_trans{p_head};
		Transaction * p_current_trans{p_head->get_next()};
		Transaction * p_current_1{nullptr};
		Transaction * p_current_2{p_head};

		//std::cout << p_current_trans;
		while(p_current_trans != nullptr) {

			//If the previous transaction's date is greater than current
			//Then you correct current's location to be in the chronological order
			if(!(*p_previous_trans < *p_current_trans)) {
				//Assign the next of the previous transaction to be the next of the current transaction
				p_previous_trans->set_next(p_current_trans->get_next());

				while(p_current_2 != p_previous_trans->get_next()) {
					if(!(*p_current_trans < *p_current_2)) {
						p_current_1 = p_current_2;
						p_current_2 = p_current_2->get_next();
					}
					else {
						if(p_current_1 == nullptr) {   //If the current 1 is still null, then update the head transaction
							p_current_trans->set_next(p_current_2);
							p_head = p_current_trans; //Update the original head
						}
						else {
							p_current_1->set_next(p_current_trans);
							p_current_trans->set_next(p_current_2);
						}
						break;
					}
				}
				p_current_trans = p_previous_trans->get_next();
				if(p_current_trans == nullptr) {
					p_current_trans = p_previous_trans;
					break;
				}
			}

			if(*p_previous_trans < *p_current_trans) {
				if(p_current_trans != nullptr) {
					//This will be used to go through the entire linked list by incrementing both pointers
					p_previous_trans = p_current_trans;
					p_current_trans = p_current_trans->get_next();
				}
			}

			p_current_1 = nullptr;
			p_current_2 = p_head;
		}
	}
}

// TASK
// update_acb_cgl(): Updates the ACB and CGL values. 
//

void History::update_acb_cgl() {
	Transaction * p_prev_trans{nullptr};
	Transaction * p_current_trans{p_head};

	double total_acb{0};
	double num_share{0};
	unsigned int total_acb_shares{0};
	double acb_per_shares_val{0};
	double cgl_value{0};

	while(p_current_trans != nullptr) {
		if(p_current_trans->get_trans_type() == true) {

			total_acb += p_current_trans->get_amount();
			p_current_trans->set_acb(total_acb);

			total_acb_shares += p_current_trans->get_shares();
			p_current_trans->set_share_balance(total_acb_shares);

			acb_per_shares_val = total_acb / total_acb_shares;
			p_current_trans->set_acb_per_share(acb_per_shares_val);

		}
		else {

			num_share = p_current_trans->get_shares();

			if(p_prev_trans != nullptr) {
				total_acb -= (num_share * p_prev_trans->get_acb_per_share());
				p_current_trans->set_acb(total_acb);
			}
			else {
				total_acb = 0;
			}

			total_acb_shares -= p_current_trans->get_shares();
			p_current_trans->set_share_balance(total_acb_shares);

			acb_per_shares_val = total_acb / total_acb_shares;
			p_current_trans->set_acb_per_share(acb_per_shares_val);

			if(p_prev_trans != nullptr) {
				cgl_value = p_current_trans->get_amount() - (num_share * p_prev_trans->get_acb_per_share());
				p_current_trans->set_cgl(cgl_value);
			}
			else {
				cgl_value = 0;
			}
		}

		p_prev_trans = p_current_trans;
		p_current_trans = p_current_trans->get_next();
	}

}

// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//

double History::compute_cgl(unsigned int year) {
	Transaction * p_current_trans{p_head};

	double total_cgl{0};

	while(p_current_trans != nullptr) {
		if(p_current_trans->get_year() == year) {
			total_cgl += p_current_trans->get_cgl();
		}
		p_current_trans = p_current_trans->get_next();
	}

	return total_cgl;
}

// TASK
// print() Print the transaction history.
//

void History::print() {
	Transaction * p_current_trans{p_head};

	std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;

	while(p_current_trans != nullptr) {
		p_current_trans->print();
		p_current_trans = p_current_trans->get_next();
	}

	std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
}

// GIVEN
// get_p_head(): Full access to the linked list.
// 
Transaction *History::get_p_head() { return p_head; }

#endif
