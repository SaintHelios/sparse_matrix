#pragma once
#include <iostream>

class sparse_matrix {
private:
	struct node {
		node* left;
		node* up;
		int row_index;
		int column_index;
		int value;
		node(int value, int row_index, int column_index, node* left = nullptr, node* up = nullptr) {
			this->value = value;
			this->left = left;
			this->up = up;
			this->row_index = row_index;
			this->column_index = column_index;
		}
	};
	size_t rows_amount;
	size_t columns_amount;
	node** rows; 
	node** columns;
private:
	bool row_is_empty(size_t);
	bool column_is_empty(size_t);
public:
	sparse_matrix( const sparse_matrix&);
	sparse_matrix(size_t, size_t);
	void add(size_t, size_t, int);
	void matrices_sum(sparse_matrix&, sparse_matrix&);
	int find(size_t, size_t) const;
	sparse_matrix operator=(const sparse_matrix&);
	~sparse_matrix();
};

void sparse_matrix::matrices_sum(sparse_matrix& right_operand, sparse_matrix& copy_to) {
	if (this->rows_amount == right_operand.rows_amount && this->columns_amount == right_operand.columns_amount) {
		for (size_t i = 0; i < this->rows_amount; ++i) {
			int to_add;
			int counter = columns_amount - 1;
			while (counter >= 0) {
				to_add = this->find(i, counter) + right_operand.find(i, counter);
				if (to_add != 0) copy_to.add(i, counter, to_add);
				--counter;
			}
		}
	}
	else {
		std::cout << "matrices have different size " << std::endl;
	}
}

sparse_matrix::sparse_matrix(const sparse_matrix& right_operand) {
	if (this->rows_amount == right_operand.rows_amount && this->columns_amount == right_operand.columns_amount) {
		for (size_t i = 0; i < this->rows_amount; ++i) {
			node* iterator = right_operand.rows[i]->left;
			while (iterator != right_operand.rows[i]) {
				if (right_operand.find(i, iterator->column_index) != 0) {
					this->add(i, iterator->column_index, iterator->value);
				}
			}
		}
	}
}
sparse_matrix sparse_matrix::operator=(const sparse_matrix& right_operand) {
	if (this->rows_amount == right_operand.rows_amount && this->columns_amount == right_operand.columns_amount) {
		for (size_t i = 0; i < this->rows_amount; ++i) {
			node* iterator = right_operand.rows[i]->left;
			while (iterator != right_operand.rows[i]) {
				if (right_operand.find(i, iterator->column_index) != 0) {
					this->add(i, iterator->column_index, iterator->value);
				}
			}
		}
	}
	return *this;
}

sparse_matrix::sparse_matrix(size_t rows_amount = 2, size_t columns_amount = 2) {
	this->rows_amount = rows_amount;
	this->columns_amount = columns_amount;
	this->rows = new node*[rows_amount];
	this->columns = new node*[columns_amount];

	for (size_t i = 0; i < rows_amount; ++i) {
		rows[i] = new node(0, i, -1);
		rows[i]->left = rows[i];
	}
	for (size_t i = 0; i < columns_amount; ++i) {
		columns[i] = new node(0, -1, i);
		columns[i]->up = columns[i];
	}
}

sparse_matrix::~sparse_matrix() {
	for (size_t i = 0; i < rows_amount; ++i) {
		if (!row_is_empty(i)) {
			node* iterator = rows[i]->left;
			node* victim;
			while (iterator != rows[i]) {
					victim = iterator;
					iterator = iterator->left;
					delete victim;
			}
		}
	}
	delete[] rows;
	delete[] columns;
}

bool sparse_matrix::row_is_empty(size_t row_index) {
	if (row_index >= 0 && row_index <= rows_amount - 1) {
		if (rows[row_index]->left == rows[row_index]) return true;
		else return false;
	}
}

bool sparse_matrix::column_is_empty(size_t column_index) {
	if (column_index >= 0 && column_index <= columns_amount - 1) {
		if (columns[column_index]->up == columns[column_index]) return true;
		else return false;
	}
}

int sparse_matrix::find(size_t row_index, size_t column_index) const {
	if ((row_index >= 0 && row_index <= rows_amount - 1) && (column_index >= 0 && column_index <= columns_amount - 1)) {
		node* iterator = rows[row_index]->left;
		while (iterator != rows[row_index] && iterator->column_index >= column_index) {
			if (iterator->column_index == column_index) return iterator->value;
			iterator = iterator->left;
		}
		return 0;
	}
	else std::cout << "this index is out of range" << std::endl;
}

void sparse_matrix::add(size_t row_index, size_t column_index, int value) {
	if ((row_index >= 0 && row_index <= rows_amount - 1) && (column_index >= 0 && column_index <= columns_amount - 1)) {
		node* left_iterator = rows[row_index]->left;
		node* add_after_left = rows[row_index];
		node* previous_left;
		while (left_iterator != rows[row_index] && left_iterator->column_index > column_index) {
			add_after_left = left_iterator;
			left_iterator = left_iterator->left;
		}
		previous_left = add_after_left->left;
		add_after_left->left = new node(value, row_index, column_index, previous_left);


		node* up_iterator = columns[column_index]->up;
		node* add_after_up = columns[column_index];
		node* previous_up;
		while (up_iterator != columns[column_index] && up_iterator->row_index > row_index) {
			add_after_up = up_iterator;
			up_iterator = up_iterator->up;
		}
		previous_up = add_after_up->up;
		add_after_up->up = add_after_left->left;
		add_after_left->left->up = previous_up;
	}
	else std::cout << "this index is out of range" << std::endl;
}
	
