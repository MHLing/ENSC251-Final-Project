#include<iostream>
#include<fstream>
#include<string>
using namespace std;

struct DATABASE {
	string struct_name;
	double* number;
	DATABASE() {
		struct_name = "i don't have name now";
		number = new double[0];
	}
};

struct Calculate_Gain {
	double* more_class = new double[2];
	double* less_class = new double[2];
};

struct Store_Gain {
	double gain;
	int column;
	int row;
	Store_Gain() {
		gain = 0;
		column = 0;
		row = 0;
	}
};

struct gain_node {
	DATABASE* database;
	gain_node* smaller_or_equal;
	gain_node* bigger;
	gain_node*parent;
	Store_Gain gain;
	int level;
	int row;
	gain_node() {
		level = 0;
		row = 0;
		database = NULL;
		smaller_or_equal = NULL;
		bigger = NULL;
		parent = NULL;
	}
	gain_node(int num_level, int num_row, DATABASE* database_A, Store_Gain gain_A) {
		level = num_level;
		row = num_row;
		database = database_A;
		gain = gain_A;
		smaller_or_equal = NULL;
		bigger = NULL;
		parent = NULL;
	}
};

string* get_class_name(DATABASE* database, int row) {
	string* class_name = new string[2];
	class_name[0] = database[0].struct_name;
	for (int i = 0; i < row; i++) {
		if (class_name[0] != database[i].struct_name) {
			class_name[1] = database[i].struct_name;
			break;
		}
	}
	return class_name;
}

double Gain_Funtion(Calculate_Gain TEMP) {
	double Gain;
	double Entropy_Main, Entropy_More, Entropy_Less;
	double p_more, p_less, p_main_1, p_main_2, p_more_1, p_more_2, p_less_1, p_less_2;
	p_main_1 = (TEMP.more_class[0] + TEMP.less_class[0]) / (TEMP.more_class[0] + TEMP.more_class[1] + TEMP.less_class[0] + TEMP.less_class[1]);
	p_main_2 = (TEMP.more_class[1] + TEMP.less_class[1]) / (TEMP.more_class[0] + TEMP.more_class[1] + TEMP.less_class[0] + TEMP.less_class[1]);
	if (TEMP.more_class[0] + TEMP.more_class[1] == 0) {
		p_more_1 = 0;
		p_more_2 = 0;
	}
	else {
		p_more_1 = (TEMP.more_class[0]) / (TEMP.more_class[0] + TEMP.more_class[1]);
		p_more_2 = (TEMP.more_class[1]) / (TEMP.more_class[0] + TEMP.more_class[1]);
	}
	if (TEMP.less_class[0] + TEMP.less_class[1] == 0) {
		p_less_1 = 0;
		p_less_2 = 0;
	}
	else {
		p_less_1 = (TEMP.less_class[0]) / (TEMP.less_class[0] + TEMP.less_class[1]);
		p_less_2 = (TEMP.less_class[1]) / (TEMP.less_class[0] + TEMP.less_class[1]);
	}
	p_more = (TEMP.more_class[0] + TEMP.more_class[1]) / (TEMP.more_class[0] + TEMP.more_class[1] + TEMP.less_class[0] + TEMP.less_class[1]);
	p_less = (TEMP.less_class[0] + TEMP.less_class[1]) / (TEMP.more_class[0] + TEMP.more_class[1] + TEMP.less_class[0] + TEMP.less_class[1]);
	Entropy_Main = -p_main_1 * log2(p_main_1) - p_main_2 * log2(p_main_2);
	Entropy_More = -p_more_1 * log2(p_more_1) - p_more_2 * log2(p_more_2);
	Entropy_Less = -p_less_1 * log2(p_less_1) - p_less_2 * log2(p_less_2);
	if (p_less_1 == 0 || p_less_2 == 0) {
		Entropy_Less = 0;
	}
	if (p_more_1 == 0 || p_more_2 == 0) {
		Entropy_More = 0;
	}
	if (p_main_1 == 0 || p_main_2 == 0) {
		Entropy_Main = 0;
	}
	Gain = Entropy_Main - p_more * Entropy_More - p_less * Entropy_Less;
	return Gain;
}

Store_Gain get_max_gain(double** Gain, int row, int column) {
	Store_Gain temp_gain;
	int max_column = 0;
	int max_row = 0;
	double temp = Gain[0][0];
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			if (temp < Gain[i][j]) {
				temp = Gain[i][j];
				max_column = j;
				max_row = i;
			}
		}
	}
	temp_gain.row = max_row;
	temp_gain.column = max_column;
	temp_gain.gain = temp;
	return temp_gain;
}

double** calculate_gain(DATABASE* database, string* class_name, int row, int column) {
	double** Gain = new double*[row];
	for (int i = 0; i < row; i++) {
		Gain[i] = new double[column];
	}
	Calculate_Gain TEMP;
	for (int i = 0; i < column; i++) {
		for (int j = 0; j < row; j++) {
			for (int z = 0; z < 2; z++) {
				TEMP.more_class[z] = 0;
				TEMP.less_class[z] = 0;
			}
			DATABASE target = database[j];
			for (int k = 0; k < row; k++) {
				DATABASE compare_target = database[k];
				if (compare_target.number[i] > target.number[i]) {
					if (compare_target.struct_name == class_name[0]) {
						TEMP.more_class[0]++;
					}
					else if (compare_target.struct_name == class_name[1]) {
						TEMP.more_class[1]++;
					}
				}
				else {
					if (compare_target.struct_name == class_name[0]) {
						TEMP.less_class[0]++;
					}
					else if (compare_target.struct_name == class_name[1]) {
						TEMP.less_class[1]++;
					}
				}
			}
			Gain[j][i] = Gain_Funtion(TEMP);
		}
	}
	return Gain;
}

int get_bigger_database_num_row(DATABASE* database, Store_Gain store_gain, int row) {
	double target = database[store_gain.row].number[store_gain.column];
	int new_row = 0;
	for (int i = 0; i < row; i++) {
			if (database[i].number[store_gain.column] > target) {
				new_row++;
			}
		}
	return new_row;
}

int get_smaller_database_num_row(DATABASE* database, Store_Gain store_gain, int row) {
	double target = database[store_gain.row].number[store_gain.column];
	int new_row = 0;
	for (int i = 0; i < row; i++) {
			if (database[i].number[store_gain.column] <= target) {
				new_row++;
			}	
	}
	return new_row;
}

DATABASE* spilt_bigger_database(DATABASE* database, int row, int comma_count, Store_Gain store_gain) {
	double target = database[store_gain.row].number[store_gain.column];
	int new_row = get_bigger_database_num_row(database, store_gain, row);
	DATABASE* sub_database = new DATABASE[new_row];
	for (int i = 0; i < new_row; i++) {
		sub_database[i].number = new double[comma_count];
	}
	int position = 0;
	for (int i = 0; i < row; i++) {
		if (database[i].number[store_gain.column] > target) {
			for (int k = 0; k < comma_count; k++) {
				sub_database[position].number[k] = database[i].number[k];
			}
			sub_database[position].struct_name = database[i].struct_name;
			position++;
		}
	}
	return sub_database;
}

DATABASE* spilt_smaller_database(DATABASE* database, int row, int comma_count, Store_Gain store_gain) {
	double target = database[store_gain.row].number[store_gain.column];
	int new_row;

	
	
	new_row = get_smaller_database_num_row(database, store_gain, row);
	

	DATABASE* sub_database = new DATABASE[new_row];
	for (int i = 0; i < new_row; i++) {
		sub_database[i].number = new double[comma_count];
	}
	int position = 0;
	for (int i = 0; i < row; i++) {
		if (database[i].number[store_gain.column] <= target) {
			for (int k = 0; k < comma_count; k++) {
				sub_database[position].number[k] = database[i].number[k];
			}
			sub_database[position].struct_name = database[i].struct_name;
			position++;
		}
	}
	return sub_database;
}

int determine_end_spilt(DATABASE* database, int row) {
	int judgment = 1;
	string temp = database[0].struct_name;
	for (int i = 0; i < row; i++) {
		if (temp != database[i].struct_name) {
			judgment = 0;
			break;
		}
	}
	return judgment;
}

gain_node* spilt(DATABASE* database, int row, int comma_count, string* class_name, gain_node* parent, int level) {
	double** Gain = calculate_gain(database, class_name, row, comma_count);
	Store_Gain SG = get_max_gain(Gain, row, comma_count);
	int bigger_new_row = get_bigger_database_num_row(database, SG, row);
	int smaller_new_row = get_smaller_database_num_row(database, SG, row);
	DATABASE* bigger_database = spilt_bigger_database(database, row, comma_count, SG);
	DATABASE* smaller_database = spilt_smaller_database(database, row, comma_count, SG);
	if (level != 1) {
		gain_node* leaf = new gain_node(level, row, database, SG);
		parent = leaf;
		leaf->parent = parent;
		if (determine_end_spilt(bigger_database, bigger_new_row) == 0) {
			leaf->bigger = spilt(bigger_database, bigger_new_row, comma_count, class_name, leaf->bigger, level + 1);
		}
		if (determine_end_spilt(smaller_database, smaller_new_row) == 0) {
			leaf->smaller_or_equal = spilt(smaller_database, smaller_new_row, comma_count, class_name, leaf->smaller_or_equal, level + 1);
		}
	}
	else {
		if (determine_end_spilt(bigger_database, bigger_new_row) == 0) {
			parent->bigger = spilt(bigger_database, bigger_new_row, comma_count, class_name, parent->bigger, level + 1);
		}
		if (determine_end_spilt(smaller_database, smaller_new_row) == 0) {
			parent->smaller_or_equal = spilt(smaller_database, smaller_new_row, comma_count, class_name, parent->smaller_or_equal, level + 1);
		}
	}
	return parent;
}

void print_smaller_formula(double gain, string* class_name, int num_class_A, int num_class_B, int level, int attributes) {
	string smaller = "<=";
	string tab = "	";
	for (int i = 0; i < level; i++) {
		cout << tab;
	}
	cout << "(Attribute: " << attributes << ") " << smaller << " " << gain;
	cout << "(" << class_name[0] << ":" << num_class_A << "  " << class_name[1] << ":" << num_class_B << ")" << endl;
}

void print_bigger_formula(double gain, string* class_name, int num_class_A, int num_class_B, int level, int attributes) {
	string bigger = ">";
	string tab = "	";
	for (int i = 0; i < level; i++) {
		cout << tab;
	}
	cout << "(Attribute: " << attributes << ") " << bigger << " " << gain;
	cout << "(" << class_name[0] << ":" << num_class_A << "  " << class_name[1] << ":" << num_class_B << ")" << endl;
}

void print(gain_node* root, string* class_name, int comma_count) {
	int target_row = root->gain.row;
	int target_col = root->gain.column;
	double gain = root->database[target_row].number[target_col];
	DATABASE* sub_smaller_database = spilt_smaller_database(root->database, root->row, comma_count, root->gain);
	DATABASE* sub_bigger_database = spilt_bigger_database(root->database, root->row, comma_count, root->gain);
	
	int bigger_row = get_bigger_database_num_row(root->database, root->gain, root->row);
	int smaller_row = get_smaller_database_num_row(root->database, root->gain, root->row);

	int class_a = 0, class_b = 0;
	for (int i = 0; i < smaller_row; i++) {
		if (sub_smaller_database[i].struct_name == class_name[0]) {
			class_a++;
		}
		else {
			class_b++;
		}
	}
	print_smaller_formula(gain, class_name, class_a, class_b, root->level, root->gain.column);
	cout << endl;

	if (root->smaller_or_equal != NULL) {
		print(root->smaller_or_equal, class_name, comma_count);
	}

	if (root->level == 1) {
		cout << "ROOT" << endl;
	}

	class_a = 0;
	class_b = 0;
	for (int i = 0; i < bigger_row; i++) {
		if (sub_bigger_database[i].struct_name == class_name[0]) {
			class_a++;
		}
		else {
			class_b++;
		}
	}
	print_bigger_formula(gain, class_name, class_a, class_b, root->level, root->gain.column);
	cout << endl;

	if (root->bigger != NULL) {
		print(root->bigger, class_name, comma_count);
	}
}

int main(int argc, char** argv) {
	fstream dataset;
	dataset.open("iris.data.txt");
	string line;
	getline(dataset, line);
	int length = line.length();
	int row = 1;
	int comma_count = 0;
	for (int i = 0; i < length; i++) {
		if (line[i] == ',') {
			comma_count++;
		}
	}
	while (true) {
		getline(dataset, line);
		if (line[0] == 0) {
			break;
		}
		row++;
	}
	dataset.close();
	dataset.open("iris.data.txt");
	char comma;
	DATABASE* database = new DATABASE[row];
	for (int i = 0; i < row; i++) {
		database[i].number = new double[comma_count];
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < comma_count; j++) {
			dataset >> database[i].number[j] >> comma;
		}
		dataset >> database[i].struct_name;
	}

	string* class_name = get_class_name(database, row);
	int level = 1;
	double** Gain = calculate_gain(database, class_name, row, comma_count);
	//for (int i = 0; i < row; i++) {
	//	for (int j = 0; j < comma_count; j++) {
	//		cout << Gain[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	Store_Gain store_gain = get_max_gain(Gain, row, comma_count);
	//cout << store_gain.column << " " << store_gain.row << " " << store_gain.gain << endl;
	gain_node* root = new gain_node(level, row, database, store_gain);
	root = spilt(database, row, comma_count, class_name, root, level);
	//cout << root->database[root->gain.row].number[root->gain.column] << endl;
	//cout << root->bigger->database[root->bigger->gain.row].number[root->bigger->gain.column] << endl;
	//cout << root->smaller_or_equal->database[root->smaller_or_equal->gain.row].number[root->smaller_or_equal->gain.column] << endl;
	//cout << root->bigger->bigger->database[root->bigger->bigger->gain.row].number[root->bigger->bigger->gain.column] << endl;
	//cout << root->bigger->smaller_or_equal->database[root->bigger->smaller_or_equal->gain.row].number[root->bigger->smaller_or_equal->gain.column] << endl;
	//cout << root->smaller_or_equal->bigger->database[root->smaller_or_equal->bigger->gain.row].number[root->smaller_or_equal->bigger->gain.column] << endl;
	//cout << root->bigger->bigger->smaller_or_equal->database[root->bigger->bigger->smaller_or_equal->gain.row].number[root->bigger->bigger->smaller_or_equal->gain.column] << endl;
	print(root, class_name, comma_count);


	for (int i = 0; i < row; i++) {
		delete[] Gain[i];
	}
	delete[] Gain;
	delete[] database;
	system("pause");
	return 0;
}
