#include "main.h"

class imp_res : public Restaurant
{
public:

	int times = 0;

	class ad_customer
	{
	public:
		customer* data;
		int time_customer;
		int place;
		ad_customer* qnext;
		ad_customer* qprev;
		ad_customer() { this->data = NULL; this->qnext = NULL; this->qprev = NULL; this->time_customer = -1; this->place = -1; }
		ad_customer(customer*& data, int time) { this->data = data; this->qnext = NULL; this->qprev = NULL; this->time_customer = time; this->place = -1; }
		~ad_customer()
		{			}
	};

	class list
	{
	public:
		ad_customer* head;
		ad_customer* tail;
		int count_list;
		list() : head(NULL), tail(NULL), count_list(0) {}
		~list()
		{
			while (count_list != 0)
			{
				ad_customer* temp = this->head;
				customer* temp_customer = this->head->data;
				if (temp_customer != NULL) { temp_customer->prev = NULL; temp_customer->next = NULL; delete temp_customer; }
				this->head = this->head->qnext;
				delete temp;
				this->count_list--;
			}
			this->head = NULL;
			this->tail = NULL;
		}
		void push_back(ad_customer*& newnode)
		{
			if (this->head == NULL) { this->head = newnode; this->tail = newnode; }
			else
			{
				tail->qnext = newnode;
				newnode->qprev = tail;
				tail = newnode;
			}
			this->count_list++;
		}

		void push_special(ad_customer*& newnode)  // danh cho in_desk de sap xep theo time tang dan
		{
			if (this->head == NULL) { this->head = newnode; this->tail = newnode; }
			else
			{
				if (newnode->time_customer > tail->time_customer)
				{
					tail->qnext = newnode;
					newnode->qprev = tail;
					tail = newnode;
				}
				else if (newnode->time_customer < head->time_customer)
				{
					newnode->qnext = head;
					head->qprev = newnode;
					head = newnode;
				}
				else
				{
					ad_customer* place = this->head->qnext;
					if (place->time_customer > newnode->time_customer)
					{
						head->qnext = newnode;
						newnode->qprev = head;
						place->qprev = newnode;
						newnode->qnext = place;
					}
					else
						for (int i = 0; i < this->count_list; i++)
						{
							ad_customer* next = place->qnext;
							if (place->time_customer < newnode->time_customer && next->time_customer > newnode->time_customer)
							{
								place->qnext = newnode;
								newnode->qprev = place;
								next->qprev = newnode;
								newnode->qnext = next;
								break;
							}
							else   place = place->qnext;
						}
				}
			}
			this->count_list++;
		}

		void pop_node(ad_customer*& oldnode) //pop node chu ko xoa
		{
			if (this->count_list == 1)
			{
				ad_customer* temp = this->head;
				temp->qnext = NULL;
				head = NULL;
				tail = NULL;
				this->count_list = 0;
			}
			else if (oldnode == this->head)
			{
				ad_customer* temp = head;
				head = head->qnext;
				head->qprev = NULL;
				temp->qnext = NULL;
				this->count_list--;
			}
			else if (oldnode == this->tail)
			{
				ad_customer* temp = this->tail;
				this->tail = this->tail->qprev;
				this->tail->qnext = NULL;
				temp->qprev = NULL;
				this->count_list--;
			}
			else
			{
				ad_customer* prev = oldnode->qprev;
				ad_customer* next = oldnode->qnext;
				prev->qnext = next;
				next->qprev = prev;
				oldnode->qnext = NULL;
				oldnode->qprev = NULL;
				count_list--;
			}

		}

		void remove_ad(ad_customer*& oldnode)  // chia delete ad_customer, ko delete data
		{
			count_list--;
			if (this->head == this->tail)
			{
				this->head->qnext = NULL;
				this->head->qprev = NULL;
				delete this->head; this->head = NULL; this->tail = NULL;
			}
			else if (oldnode == this->head)
			{
				this->head = this->head->qnext;
				this->head->qprev = NULL;
				oldnode->qnext = NULL;
				oldnode->qprev = NULL;
			}
			else if (oldnode == this->tail)
			{
				this->tail = this->tail->qprev;
				this->tail->qnext = NULL;
				oldnode->qnext = NULL;
				oldnode->qprev = NULL;
			}
			else
			{
				ad_customer* prev = oldnode->qprev;
				ad_customer* next = oldnode->qnext;
				prev->qnext = next;
				next->qprev = prev;
				oldnode->qnext = NULL;
				oldnode->qprev = NULL;
			}
			delete oldnode;
		}

		void remove(ad_customer*& oldnode) // xy ly cho list o ham domain_expansion: xoa tat ca data ben duoi	: chi danh cho o wailtinglist
		{
			count_list--;
			if (head == tail && head == oldnode)
			{
				if (head->data != NULL)			 delete head->data;		  // trong desk
				delete head;
				head = NULL; tail = NULL;
			}
			else if (oldnode == tail)
			{
				ad_customer* temp = tail;
				tail = tail->qprev;
				tail->qnext = NULL;
				temp->qprev = NULL;
				if (temp->data != NULL) delete temp->data;
				delete temp;
			}
			else if (oldnode == head)
			{
				ad_customer* temp = head;
				head = head->qnext;
				head->qprev = NULL;
				temp->qnext = NULL;
				if (temp->data != NULL) delete temp->data;
				delete temp;
			}
			else
			{
				ad_customer* prev = oldnode->qprev;
				ad_customer* next = oldnode->qnext;
				prev->qnext = next;
				next->qprev = prev;
				oldnode->qnext = NULL;
				oldnode->qprev = NULL;
				if (oldnode->data != NULL) delete oldnode->data;
				delete oldnode;
			}
		}
	};

	// ########################################### ADDING SOME VARIABLES

	customer* X = NULL; 			// circle desk
	int count_thuatsu = 0;			// so khach hang la thuat su duoc ngoi vao desk (ngoi vao ban)
	int count_oanlinh = 0;			// so khach hang la oan linh duoc ngoi vao desk (ngoi vao ban)
	list* in_desk = new list();	// danh sach theo thoi gian vao nha hang
	list* w_list = new list();		// hang doi - waiting customer

	// ########################################### ADDING SOME FUCNTION

	void X_remove_customer(customer*& ptr) // yes, ham nay delete luon ca customer
	{
		// hay rang buoc ptr khac null moi duoc remove
		if (ptr == NULL)	return;//throw out_of_range(" ptr is NULL");
		else
		{
			if (in_desk->count_list == 1) { count_thuatsu = 0; count_oanlinh = 0; X->next = NULL; X->prev = NULL; delete ptr; this->X = NULL; }
			else
			{
				customer* next = ptr->next;
				customer* prev = ptr->prev;
				prev->next = next;
				next->prev = prev;
				if (ptr->energy > 0)
				{
					ptr->next = NULL;  	ptr->prev = NULL;
					delete ptr; 		this->count_thuatsu--;	this->X = next;
				}
				else
				{
					ptr->next = NULL;  	ptr->prev = NULL;
					delete ptr;	this->count_oanlinh--;	this->X = prev;
				}
			}
			ptr = NULL;
		}
	}

	void remove_super_customer(ad_customer*& oldnode)  // xoa that su tat ca data
	{
		if (oldnode->place == 1) // dang o trong in_desk (X)
		{
			X_remove_customer(oldnode->data);
			in_desk->pop_node(oldnode);
			delete oldnode;
		}
		else
		{
			delete oldnode->data;
			w_list->pop_node(oldnode);
			delete oldnode;
		}
	}

	// push_customer xu ly tren X; da xu ly count < MAXSIZE

	void push_customer(customer*& new_customer)
	{
		int energy = new_customer->energy;
		if (in_desk->count_list == 0)
		{
			X = new_customer;
			X->next = X;
			X->prev = X;
		}
		else if (in_desk->count_list < MAXSIZE / 2)
		{
			if (energy >= X->energy)
			{
				customer* temp = X->next;
				X->next = new_customer;
				new_customer->prev = X;
				new_customer->next = temp;
				temp->prev = new_customer;
				X = new_customer;
			}
			else
			{
				customer* temp = X->prev;
				X->prev = new_customer;
				new_customer->next = X;
				new_customer->prev = temp;
				temp->next = new_customer;
				X = new_customer;
			}
		}
		else
		{
			int max_dif = abs(X->energy - energy);
			customer* temp = X->next;
			customer* place = X;
			bool rotate_clock = false;
			if (energy - place->energy > 0) rotate_clock = true;
			while (temp != X)
			{
				if (abs(temp->energy - energy) > max_dif)
				{
					max_dif = abs(temp->energy - energy);
					if (energy - temp->energy > 0) rotate_clock = true;
					else rotate_clock = false;
					place = temp;
				}
				temp = temp->next;
			}
			if (rotate_clock)
			{
				customer* next_temp = place->next;
				place->next = new_customer;
				new_customer->prev = place;
				new_customer->next = next_temp;
				next_temp->prev = new_customer;
				X = new_customer;
			}
			else
			{
				customer* prev_temp = place->prev;
				place->prev = new_customer;
				new_customer->next = place;
				new_customer->prev = prev_temp;
				prev_temp->next = new_customer;
				X = new_customer;
			}
		}
		if (new_customer->energy > 0) this->count_thuatsu++; else this->count_oanlinh++;
	}


	ad_customer* skipp_back(ad_customer*& A, int skip)
	{
		ad_customer* result = A;
		while (result != NULL && skip != 0)
		{
			result = result->qprev;
			skip--;
		}
		return result;
	}

	ad_customer* skipp_next(ad_customer*& A, int skip, int& runner_index, int index_max)
	{
		runner_index += skip;
		if (runner_index > index_max) return NULL;
		ad_customer* result = A;
		while (skip != 0)
		{
			result = result->qnext;
			skip--;
		}
		return result;
	}

	int insertsort(ad_customer*& max, ad_customer*& begin, int incr)
	{
		int mini_swap = 0;
		if (begin == NULL) return 0;
		ad_customer* prev = begin;
		// runner is the next incr after begin
		ad_customer* runner = begin;
		for (int i = 0; i < incr; i++)
		{
			runner = runner->qnext;
			if (runner == NULL) break;
			else if (runner == max && i != incr - 1) { runner = NULL; break; }
		}

		while (runner)
		{
			int val_runner = abs(runner->data->energy);
			// kiểm tra 
			if (val_runner > abs(prev->data->energy) || (val_runner == abs(prev->data->energy) && runner->time_customer < prev->time_customer)) // phía trước abs nhỏ hơn => vi phạm chuỗi giảm	 ; time nho hon la lon hon
			{
				// step A: find place for runner
				ad_customer* place = runner;
				while (true)
				{
					//swap truoc 1 lan, vi chac chan
					int time_place = place->time_customer;
					customer* data_swap = place->data;
					place->data = prev->data;
					place->time_customer = prev->time_customer;
					prev->data = data_swap;
					prev->time_customer = time_place;
					mini_swap++;

					place = prev;

					// backward prev
					for (int i = 0; i < incr; i++)
					{
						prev = prev->qprev;
						if (prev == NULL) break;
					}
					if (prev == NULL) break; // temp truoc do da la node dau tien cua day runner hien ta							
					else if (abs(place->data->energy) < abs(prev->data->energy) || (abs(place->data->energy) == abs(prev->data->energy) && place->time_customer > prev->time_customer)) break;	// node lien truoc lon hon => thoa
				}
			}
			prev = runner;
			// next runner: neu runner di qua max => runner = NULL => dung vong while
			if (runner == max)
			{
				runner = NULL;	break;
			}

			for (int i = 0; i < incr; i++)
			{
				runner = runner->qnext;
				if (runner == NULL) break;
				else if (runner == max && i != incr - 1)
				{
					runner = NULL; break;
				}

			}
		}
		return mini_swap;
	}

	// ########################################### IMPLEMENT FUNCTION
	imp_res() {};

	void RED(string name, int energy)
	{
		if (energy != 0)
		{
			bool not_duplicate_name = true;
			// kiem tra trung ten trong stt_comein
			ad_customer* temp_check = in_desk->head;
			while (temp_check != NULL)
			{
				if (temp_check->data->name == name)
				{
					not_duplicate_name = false;
					break;
				}
				else temp_check = temp_check->qnext;
			}
			temp_check = w_list->head;
			while (temp_check != NULL)
			{
				if (temp_check->data->name == name)
				{
					not_duplicate_name = false;
					break;
				}
				else temp_check = temp_check->qnext;
			}

			// if not duplicate name
			if (not_duplicate_name)
			{
				this->times++;
				customer* new_customer = new customer(name, energy, NULL, NULL);
				ad_customer* ad_new = new ad_customer(new_customer, this->times);
				// ad ad_new to stt_comein 				
				if (in_desk->count_list < MAXSIZE)	  // cho vao in_desk
				{	 // add new_customer to X
					push_customer(new_customer);
					in_desk->push_special(ad_new);
					ad_new->place = 1;
				}
				else if (w_list->count_list < MAXSIZE)	  // cho vao waiting 					
				{
					w_list->push_back(ad_new);
					ad_new->place = 2;
				}	// add ad_new2 to w_list
				else		  // ca 2 deu full  => duoi
				{
					delete new_customer;
					delete 	ad_new;
					this->times--;
				}
			}
		}
	}
	void BLUE(int num)
	{
		if (in_desk->count_list > 0)
		{
			while (num > 0 && in_desk->count_list > 0)
			{
				ad_customer* remove_node = in_desk->head;
				remove_super_customer(remove_node);
				num--;
			}
			if (w_list->count_list > 0 && in_desk->count_list < MAXSIZE)
			{
				while (w_list->count_list > 0 && in_desk->count_list < MAXSIZE)
				{
					ad_customer* temp = w_list->head;
					push_customer(w_list->head->data);
					temp->place = 1;
					w_list->pop_node(temp);
					in_desk->push_special(temp);
				}
			}
		}
	}

	void PURPLE()
	{
		// step1: - find and assign element in w_queue have max abs(energy)
		if (w_list->count_list > 1)
		{
			int index = 0;
			ad_customer* temp = w_list->head->qnext;
			ad_customer* max_customer = w_list->head;
			int index_max = 0;
			int time_max = w_list->head->time_customer;
			int max_energy = abs(max_customer->data->energy);
			while (temp != NULL)
			{
				index++;
				int val = abs(temp->data->energy);

				if (val > max_energy || (val == max_energy && temp->time_customer > time_max))
				{
					max_energy = val;
					max_customer = temp;
					index_max = index;
					time_max = temp->time_customer;
				}

				temp = temp->qnext;
			}
			// step 1 thu duoc customer: max_customer va int index_max		
			// step2: áp dụng shellsort, bat dau tu node-max den node dau cua node_list;
			if (index_max != 0) // nếu max ko nằm ở vị trí đầu tiên của hàng chờ
			{
				int number_swap = 0;
				// voi moi increment: ex: 8,4,2
				for (int incr = (index_max + 1) / 2; incr > 2; incr /= 2)
				{
					// khoi dau luon la node star_waiting_list
					ad_customer* begin_node = w_list->head;

					//=> với từng cụm ex: (0,4,8); (1,5,9); ...
					for (int j = 0; j < incr; j++)
					{
						number_swap += insertsort(max_customer, begin_node, incr);
						begin_node = begin_node->qnext;
					}
				}
				number_swap += insertsort(max_customer, w_list->head, 1);

				//	cout << endl << "number swap: " << number_swap << endl;
					// sau khi sắp xếp lại hàng chờ => lệnh Blue
				int num_blue = number_swap % MAXSIZE;
				// sau khi sắp xếp lại hàng chờ => lệnh Blue
				if (num_blue > 0) BLUE(num_blue);
			}
		}
	}
	void REVERSAL()
	{
		if (in_desk->count_list > 2)
		{
			customer* temp2 = NULL;
			// backup_info cuar X der truy ra X suy nay
			customer** startX = &X;
			customer* X_X = X;

			// be gay lien ket vong:
			customer* next_X = X->next;
			X->next = NULL;
			next_X->prev = NULL;

			// xac dinh cac moc:
			customer* first_oanlinh = X;
			customer* end_oanlinh = next_X;
			customer* first_thuatsu = X;
			customer* end_thuatsu = next_X;
			customer* prev_first = NULL;
			customer* next_first = NULL;
			customer* prev_end = NULL;
			customer* next_end = NULL;

			if (count_oanlinh > 0)
			{
				if (first_oanlinh->energy > 0)
					while (first_oanlinh->energy > 0)  first_oanlinh = first_oanlinh->prev;

				if (end_oanlinh->energy > 0)
					while (end_oanlinh->energy > 0)  end_oanlinh = end_oanlinh->next;
			}
			else
			{
				first_oanlinh = NULL;
				end_oanlinh = NULL;
			}
			if (count_thuatsu > 0)
			{
				if (first_thuatsu->energy < 0)
					while (first_thuatsu->energy < 0)  first_thuatsu = first_thuatsu->prev;

				if (end_thuatsu->energy < 0)
					while (end_thuatsu->energy < 0)  end_thuatsu = end_thuatsu->next;
			}
			else
			{
				first_thuatsu = NULL;
				end_thuatsu = NULL;
			}


			if (X == first_oanlinh)
			{
				X_X = end_oanlinh;
				if (next_X == end_oanlinh) next_X = first_oanlinh;
				//	else next_X = end_thuatsu;
				else next_X = first_thuatsu;
			}
			else
			{
				X_X = end_thuatsu;
				if (next_X == end_thuatsu) next_X = first_thuatsu;
				//else next_X = first_thuatsu;
				else next_X = first_oanlinh;
			}

			if (count_oanlinh > 1)
			{
				// tien hanh reverse:
				while (first_oanlinh->name != end_oanlinh->name)
				{
					// break and swap lien ket:
					prev_first = first_oanlinh->prev;
					prev_end = end_oanlinh->prev;
					next_first = first_oanlinh->next;
					next_end = end_oanlinh->next;


					first_oanlinh->prev = prev_end;		 //absolute				
					end_oanlinh->next = next_first;		 //absolute
					if (prev_first == end_oanlinh) end_oanlinh->prev = first_oanlinh;
					else
					{
						if (prev_first != NULL)	prev_first->next = end_oanlinh;
						end_oanlinh->prev = prev_first;
					}

					if (next_first != NULL)	next_first->prev = end_oanlinh;	  		  //absolute
					if (prev_end != NULL)	prev_end->next = first_oanlinh;			  //absolute
					if (first_oanlinh == next_end) first_oanlinh->next = end_oanlinh;
					else
					{
						if (next_end != NULL)	next_end->prev = first_oanlinh;
						first_oanlinh->next = next_end;
					}

					// get new fisr, end:
					customer* temp = first_oanlinh;
					first_oanlinh = end_oanlinh->prev;


					if (first_oanlinh->energy > 0)
						while (first_oanlinh->energy > 0)  first_oanlinh = first_oanlinh->prev;
					if (first_oanlinh == temp) break; // da swap cap oan linh cuoi cung						
					end_oanlinh = temp->next;
					if (end_oanlinh->energy > 0)
						while (end_oanlinh->energy > 0)  end_oanlinh = end_oanlinh->next;
				}
			}
			if (count_thuatsu > 1)
			{


				while (first_thuatsu->name != end_thuatsu->name)
				{
					// break and swap lien ket:
					prev_first = first_thuatsu->prev;
					prev_end = end_thuatsu->prev;
					next_first = first_thuatsu->next;
					next_end = end_thuatsu->next;

					first_thuatsu->prev = prev_end;	 //absolute
					end_thuatsu->next = next_first;	 //absolute
					if (prev_first == end_thuatsu) end_thuatsu->prev = first_thuatsu;
					else
					{
						if (prev_first != NULL)	prev_first->next = end_thuatsu;
						end_thuatsu->prev = prev_first;
					}

					if (next_first != NULL)	next_first->prev = end_thuatsu;		  //absolute
					if (prev_end != NULL)	prev_end->next = first_thuatsu;		  //absolute
					if (first_thuatsu == next_end) first_thuatsu->next = end_thuatsu;
					else
					{
						if (next_end != NULL)	next_end->prev = first_thuatsu;
						first_thuatsu->next = next_end;
					}


					// get new fisr, end:
					customer* temp = first_thuatsu;
					first_thuatsu = end_thuatsu->prev;

					if (first_thuatsu->energy < 0)
						while (first_thuatsu->energy < 0)  first_thuatsu = first_thuatsu->prev;
					if (first_thuatsu == temp) break; // da swap cap oan linh cuoi cung						
					end_thuatsu = temp->next;
					if (end_thuatsu->energy < 0)
						while (end_thuatsu->energy < 0)  end_thuatsu = end_thuatsu->next;
				}
			}
			// noi lai lien ket
			X_X->next = next_X;
			next_X->prev = X_X;
			// hoan tra dia chi cho X
			X = *startX;
		}
	}
	void UNLIMITED_VOID()
	{
		if (in_desk->count_list >= 4)
		{
			// first sum_value:
			int index_depa = 0;
			customer* depa = X;
			int sum = depa->energy;
			customer* temp = depa;
			for (int i = 1; i < 4; i++) {
				temp = temp->next;
				sum += temp->energy;
			}
			int minest_val = sum;

			// VONG 1: BAT DAU TU X
			int min_before = depa->energy;
			customer* idx_star = depa;
			customer* idx_end = depa;
			for (int i = 1; i < 4; i++) idx_end = idx_end->next; // element last of combo 4
			customer* runner_end = idx_end->next;
			customer* runner_begin = depa->next;
			customer* check_point = depa;

			int temp_length = 4;
			int incr = 1;

			customer* temp2 = idx_star;
			while (runner_end != check_point)
			{
				sum = sum + runner_end->energy - runner_begin->prev->energy;
				if (sum + min_before <= minest_val && sum + min_before <= sum)	  // trường hợp này in_before là số âm, và tổng sum + min_before nhỏ hơn minest_val => mở rộng địa bàng lên
				{
					idx_star = temp2;
					idx_end = runner_end;
					minest_val = sum + min_before;
					// truong hop nay mo rong length
					temp_length = 4 + incr;
				}
				else if (((sum == minest_val && temp_length == 4) || sum < minest_val) && sum < sum + min_before) {	// truong hop nay la min_before dương, "sum" là số nhỏ hơn miniest_val
					// if ở trên là chỉ xét khi sum_val và leng bằng, hoặc sum_val nhỏ hơn
					idx_end = runner_end;
					idx_star = runner_begin;
					minest_val = sum;
					temp_length = 4; // roi vao truong hop nay la reset lai chieu dai
				}
				// cap nhat 
				if (runner_begin->energy < min_before + runner_begin->energy)
				{
					{min_before = runner_begin->energy;	incr = 1;	temp2 = runner_begin;	 }					// thay doi minbefore
				}
				else { min_before = min_before + runner_begin->energy; incr++; }			  // mở rộng minbefore
				runner_begin = runner_begin->next;
				runner_end = runner_end->next;
				//cout << " min_before: " << min_before << " minest_val= " << minest_val << endl;
			}
			//cout << " @@@@@@@@@@V1: begin: " << idx_star->name << "-"<< idx_star->energy << idx_end->name << "-" << idx_end->energy << " minest_val= " << minest_val << endl; 

			customer* final_idx_star = idx_star;
			customer* final_idx_end = idx_end;
			int final_minest_sum = minest_val;
			int final_length = temp_length;

			// CHO LAP THEM N-1 VONG NUA:
			//  tham so gioi han
			int ini_index_depa = 1;
			int index_star = 0;	// index_star khong duoc qua count_list
			depa = X->next;
			while (depa != X)
			{

				sum = depa->energy;
				customer* temp = depa;
				for (int i = 1; i < 4; i++) {
					temp = temp->next;
					sum += temp->energy;
				}
				minest_val = sum;
				min_before = depa->energy;
				idx_star = depa;
				// bo sung  bien gioi han
				index_star = ini_index_depa + 1;

				idx_end = depa;
				for (int i = 1; i < 4; i++) idx_end = idx_end->next; // element last of combo 4
				runner_end = idx_end->next;
				runner_begin = depa->next;
				check_point = depa;
				// reset for new tune
				temp_length = 4;
				incr = 1;

				temp2 = idx_star;
				while (runner_end != check_point)
				{

					sum = sum + runner_end->energy - runner_begin->prev->energy;
					if (sum + min_before <= minest_val && sum + min_before <= sum)	  // trường hợp này in_before là số âm, và tổng sum + min_before nhỏ hơn minest_val => mở rộng địa bàng lên
					{
						idx_star = temp2;
						idx_end = runner_end;
						minest_val = sum + min_before;
						// truong hop nay mo rong length
						temp_length = 4 + incr;
					}
					else if (((sum == minest_val && temp_length == 4) || sum < minest_val) && sum < sum + min_before) {	// truong hop nay la min_before dương, "sum" là số nhỏ hơn miniest_val
						// if ở trên là chỉ xét khi sum_val và leng bằng, hoặc sum_val nhỏ hơn
						if (index_star >= in_desk->count_list)	  break;
						idx_end = runner_end;
						idx_star = runner_begin;
						minest_val = sum;
						temp_length = 4; // roi vao truong hop nay la reset lai chieu dai
					}
					// cap nhat 
					if (runner_begin->energy < min_before + runner_begin->energy)
					{
						{min_before = runner_begin->energy; index_star += incr;	incr = 1; temp2 = runner_begin;		 }					// thay doi minbefore
					}
					else {
						min_before = min_before + runner_begin->energy; incr++;
					}			  // mở rộng minbefore
					runner_begin = runner_begin->next;
					runner_end = runner_end->next;
				}
				//cout << " @@@@@@@@@@V2: step: " << idx_star->name << "-" << idx_star->energy << idx_end->name << "-" << idx_end->energy << " minest_val= " << minest_val << endl;

				depa = depa->next;
				ini_index_depa++;
				if ((minest_val < final_minest_sum) || (minest_val == final_minest_sum && temp_length >= final_length))
				{
					final_minest_sum = minest_val;
					final_idx_end = idx_end;
					final_idx_star = idx_star;
					final_length = temp_length;
				}
			}

			//cout << " @@@@@@@@@@V1: final: " << final_idx_star->name << "-" << final_idx_star->energy << final_idx_end->name << "-" << final_idx_end->energy << " minest_val= " << final_minest_sum << endl;

			// tim vi tri min:
			customer* place_minest = final_idx_star;
			temp = final_idx_star->next;
			while (temp != final_idx_end->next)
			{
				if (temp->energy < place_minest->energy)
				{
					place_minest = temp;
				}
				temp = temp->next;
			}
			if (place_minest == final_idx_star) // in tu dau
			{
				if (place_minest == final_idx_end->next)
				{
					place_minest->print();
					place_minest = place_minest->next;
					while (place_minest != final_idx_star)
					{
						place_minest->print();
						place_minest = place_minest->next;
					}
				}
				else
					while (place_minest != final_idx_end->next)
					{
						place_minest->print();
						place_minest = place_minest->next;
					}
			}
			else
			{
				// in chia lam 2 giai doan:
				check_point = place_minest;
				while (place_minest != final_idx_end->next)
				{
					place_minest->print();
					place_minest = place_minest->next;
				}
				place_minest = final_idx_star;
				while (place_minest != check_point)
				{
					place_minest->print();
					place_minest = place_minest->next;
				}
			}
		}
	}
	void DOMAIN_EXPANSION()
	{
		// tinh tong trong danh sach stt_comin cho nhanh:
		if (in_desk->count_list + w_list->count_list > 1)
		{
			// tinh sum moi team
			int sum_thuatsu = 0;
			int sum_oanlinh = 0;
			ad_customer* temp_super = in_desk->head;
			for (int i = 0; i < in_desk->count_list; i++)
			{
				if (temp_super->data->energy < 0) sum_oanlinh += temp_super->data->energy;
				else sum_thuatsu += temp_super->data->energy;
				temp_super = temp_super->qnext;
			}
			temp_super = w_list->head;
			for (int i = 0; i < w_list->count_list; i++)
			{
				if (temp_super->data->energy < 0) sum_oanlinh += temp_super->data->energy;
				else sum_thuatsu += temp_super->data->energy;
				temp_super = temp_super->qnext;
			}
			// tao danh sach khach hang bi duoi
			list* ds_khachhangbiduoi = new list();

			if (sum_thuatsu >= abs(sum_oanlinh)) 	// thuat su manh hon => duoi oan linh
			{
				temp_super = in_desk->head;
				while (temp_super != NULL)
				{
					if (temp_super->data->energy < 0)
					{
						// giu lai thong tin cua khach hang remove:

						customer* backup_info = new customer(temp_super->data->name, temp_super->data->energy, NULL, NULL);
						ad_customer* temp_ad = new ad_customer(backup_info, temp_super->time_customer);
						ds_khachhangbiduoi->push_back(temp_ad);
						// xoa di node lien quan
						ad_customer* node_remove = temp_super;
						temp_super = temp_super->qnext;
						remove_super_customer(node_remove);
					}
					else temp_super = temp_super->qnext;
				}
				temp_super = w_list->head;
				while (temp_super != NULL)
				{
					if (temp_super->data->energy < 0)
					{
						// giu lai thong tin cua khach hang remove:

						customer* backup_info = new customer(temp_super->data->name, temp_super->data->energy, NULL, NULL);
						ad_customer* temp_ad = new ad_customer(backup_info, temp_super->time_customer);
						ds_khachhangbiduoi->push_back(temp_ad);
						// xoa di node lien quan
						ad_customer* node_remove = temp_super;
						temp_super = temp_super->qnext;
						remove_super_customer(node_remove);
					}
					else temp_super = temp_super->qnext;
				}
			}
			else 								// => duoi thuat su
			{
				temp_super = in_desk->head;
				while (temp_super != NULL)
				{
					if (temp_super->data->energy > 0)
					{
						// giu lai thong tin cua khach hang remove:

						customer* backup_info = new customer(temp_super->data->name, temp_super->data->energy, NULL, NULL);
						ad_customer* temp_ad = new ad_customer(backup_info, temp_super->time_customer);
						ds_khachhangbiduoi->push_back(temp_ad);
						// xoa di node lien quan
						ad_customer* node_remove = temp_super;
						temp_super = temp_super->qnext;
						remove_super_customer(node_remove);
					}
					else temp_super = temp_super->qnext;
				}
				temp_super = w_list->head;
				while (temp_super != NULL)
				{
					if (temp_super->data->energy > 0)
					{
						// giu lai thong tin cua khach hang remove:

						customer* backup_info = new customer(temp_super->data->name, temp_super->data->energy, NULL, NULL);
						ad_customer* temp_ad = new ad_customer(backup_info, temp_super->time_customer);
						ds_khachhangbiduoi->push_back(temp_ad);
						// xoa di node lien quan
						ad_customer* node_remove = temp_super;
						temp_super = temp_super->qnext;
						remove_super_customer(node_remove);
					}
					else temp_super = temp_super->qnext;
				}
			}
			//in danh sach khach bi duoi + remove node (ca ad_customer va customer => su dung remove_deep)
			// phai sort lai theo time

			while (ds_khachhangbiduoi->head != NULL)
			{
				ad_customer* remove_node = ds_khachhangbiduoi->head;
				temp_super = remove_node->qnext;
				while (temp_super != NULL)
				{
					if (temp_super->time_customer > remove_node->time_customer)	remove_node = temp_super;	// lay khach hang den nha hang tre nhat (gan day nhat) => time lon nhat
					temp_super = temp_super->qnext;
				}
				remove_node->data->print();
				ds_khachhangbiduoi->remove(remove_node);
			}
			// xoa ds_khachangbiduoi
			delete ds_khachhangbiduoi;
		}

		if (w_list->count_list > 0 && in_desk->count_list < MAXSIZE)
		{
			while (w_list->count_list > 0 && in_desk->count_list < MAXSIZE)
			{
				ad_customer* temp = w_list->head;
				push_customer(w_list->head->data);
				temp->place = 1;
				w_list->pop_node(temp);
				in_desk->push_special(temp);
			}
		}
	}

	void LIGHT(int num)
	{
		if (num == 0) //// PHU THUOC VAO CAU TRUC CUA HANG DOI
		{
			ad_customer* temp = w_list->head;
			int count_w = w_list->count_list;
			while (count_w > 0)
			{
				temp->data->print();
				temp = temp->qnext;
				count_w--;
			}
		}
		else if (X != NULL)
		{
			customer* temp = this->X;
			temp->print();
			if (num > 0)
			{
				temp = temp->next;
				while (temp != this->X)
				{
					temp->print();
					temp = temp->next;
				}
			}
			else
			{
				temp = temp->prev;
				while (temp != this->X)
				{
					temp->print();
					temp = temp->prev;
				}
			}
		}
	}

	////////// ham huy, don va xoa rac:
	~imp_res()
	{

		delete w_list;
		delete in_desk;

		//cap nhat cac thong so phu
		this->count_oanlinh = 0;
		this->count_thuatsu = 0;
		this->times = 0;
	}
};
