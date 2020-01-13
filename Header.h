#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>
#include "../Control_list2/opencv2/opencv.hpp"
#include <Windows.h>
#include <fstream>
#include <SQLiteCpp/SQLiteCpp.h>
#include <winsqlite/winsqlite3.h>
#define PEOPLE 13
#define COLS 4
#define MAX_ERR_REGOGN 7

using namespace cv;
using namespace std;


string ary[PEOPLE][COLS];
int flag = 0;

class _sql{

	protected:
		sqlite3 *db = 0;
		sqlite3_stmt *stmtU;
		sqlite3_stmt *stmtUE;
		int err;

	public:
		
		_sql() {

			sqlite3_open16(L"list.dblite", &db);

		}


		int get_count_err() {
		
			return err;
		}
		int _sqlCreateTable() {

			string key;
			int work;
			ifstream fin("sql_list.txt");

			if(fin.is_open()){
				fin >> key;
				fin.close();
				if (key[0] == '0') {

					ofstream fout("sql_list.txt");
					fout << "10";
					fout.close();
					work = 1;

				}
				else {
					work = 0;
				}

			}
			else {

				ofstream fout("sql_list.txt");
				fout << "10";
				fout.close();

				work = 1;
			}
			if (work == 1) {
				const wchar_t* _SQLquery = L"CREATE TABLE IF NOT EXISTS 'Users'(id INTEGER PRIMARY KEY,ini VARCHAR(255));";
				sqlite3_prepare16(db, _SQLquery, -1, &stmtU, 0);
				sqlite3_step(stmtU);
				sqlite3_reset(stmtU);
				sqlite3_finalize(stmtU);

				_SQLquery = L"CREATE TABLE IF NOT EXISTS 'ListUs'(id INTEGER PRIMARY KEY,nameid INTEGER(25), time DOOBLE(30), turnout INTEGER(2));";
				sqlite3_prepare16(db, _SQLquery, -1, &stmtU, 0);

				sqlite3_step(stmtU);
				sqlite3_reset(stmtU);
				sqlite3_finalize(stmtU);

				
			}

			return 0;
		}

		int CreateUser()
		{

			string key;
			int work;
			ifstream fin("sql_list.txt");

			if (fin.is_open()) {
				fin >> key;
				fin.close();
				if (key[0] == '0') {

					if (_sqlCreateTable() == 0) {


						work=1;
					}

				}
				else if (key[0]== '1' && key[0] == '0')  {
					ofstream fout("sql_list.txt");
					fout << "11";
					fout.close();
					work = 1;

				}
				else {
					work = 0;
				}

			}
			else {

				ofstream fout("sql_list.txt");
				fout << "00";
				fout.close();
				_sqlCreateTable();
				ofstream fout2("sql_list.txt");
				fout2 << "11";
				fout2.close();

				work = 1;

			}

			if (work == 1) {
				const wchar_t* _SQLquery[PEOPLE + 1] = { L"INSERT INTO 'Users'(id,ini) VALUES(?,'Аветов Артем Сократович');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Александров Александр Александрович');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Балаенков Владислав Андреевич');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Балахнов Николай Евгеньевич');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Богомолов Вячеслав Павлович');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Денисов Иван Иванович');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Игнатьев Андрей Александрович');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Костик Ален Андреевич');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Кулакова Анна Сергеевна');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Липатов Илья Владимирович');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Напреенко Владислав Сергеевич');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Никитина Мария Николаевна');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Панов Роман Дмитриевич');",
					L"INSERT INTO 'Users'(id,ini) VALUES(?,'Нераспознано');"
				};

				for (int u = 0; u < PEOPLE + 1; u++) {

					sqlite3_prepare16(db, _SQLquery[u], -1, &stmtU, 0);
					sqlite3_step(stmtU);
					sqlite3_reset(stmtU);
					sqlite3_finalize(stmtU);


				}

				
			}
			
			return 0;
		}

		void checkCreate() {

			string key;
			
			ifstream fin("sql_list.txt");

			if (fin.is_open()) {
				fin >> key;
				fin.close();
				if (key != "11") {

					CreateUser();

				}

			}
			else {

				CreateUser();
			}

		}

		void _sqlSelectList()
		{

			checkCreate();
			int rc = 0;
			rc = sqlite3_prepare16(db, L"SELECT DISTINCT * FROM 'ListUs'", -1, &stmtU, 0);
			//sqlite3_step(stmt);

			int dat = -1;
			int buf = -1;
			double  buf2;
			int buf3 = -1;
			while (sqlite3_step(stmtU) == SQLITE_ROW)
			{
				dat = (int)sqlite3_column_int(stmtU, 0);
				buf = (int)sqlite3_column_int(stmtU, 1);
				buf2 = (double)sqlite3_column_double(stmtU, 2);
				buf3 = (int)sqlite3_column_int(stmtU, 3);


				int rc2 = 0;
				rc2 = sqlite3_prepare16(db, L"SELECT * FROM 'Users' WHERE id=?;", -1, &stmtUE, 0);
				sqlite3_bind_int(stmtUE,1,buf);

				sqlite3_step(stmtUE);

				int d = (int)sqlite3_column_int(stmtUE, 0);
				string  name = (char *)sqlite3_column_text(stmtUE,1);



				cout << dat << "|         |" << name << "|         |" << buf2 << "|         |" << buf3 << endl;

				sqlite3_reset(stmtUE);
				sqlite3_finalize(stmtUE);



			}
			sqlite3_reset(stmtU);
			sqlite3_finalize(stmtU);
		}


		string f2time(float buf2) {


			string time_s;

			if (buf2 == 0.0) {

			time_s = "--:--";

			}else if (buf2 - floor(buf2) > 0.6 || buf2>24.0) {
					
				time_s = "--:--";
				
				}else {
				time_s = to_string(buf2);

				int razd = time_s.find('.');
				if (razd == 1) {
					time_s = "0" + time_s;
				}
				replace(time_s.begin(), time_s.end(), '.', ':');
				time_s = time_s.substr(0, 5);

				while (time_s.length() < razd + 3) {

					time_s += "0";
				}
			}
			return time_s;


		}

		void _sqlResParam( const char * h) {

			checkCreate();
			int buf = -1;
			double  buf2;

			int rc = 0;

			if (h == "Arrive on time:") {

				rc = sqlite3_prepare16(db, L"SELECT DISTINCT nameid,time FROM 'ListUs' WHERE turnout=1 AND time <= 9.0", -1, &stmtU, 0);//пришедшие вовремя

			}
			else if (h == "Laters:") {

				rc = sqlite3_prepare16(db, L"SELECT DISTINCT nameid,time FROM 'ListUs' WHERE turnout=1 AND time > 9.0", -1, &stmtU, 0);//опоздавшие

			}
			else if (h == "Missing:") {

				rc = sqlite3_prepare16(db, L"SELECT DISTINCT nameid,time FROM 'ListUs' WHERE turnout=0", -1, &stmtU, 0);//не пришедшие

			}

			cout << h << endl;
			while (sqlite3_step(stmtU) == SQLITE_ROW)
			{


				buf = (int)sqlite3_column_int(stmtU, 0);
				buf2 = (double)sqlite3_column_double(stmtU, 1);

				string time_s = f2time(buf2);

				int rc2 = 0;
				rc2 = sqlite3_prepare16(db, L"SELECT * FROM 'Users' WHERE id=?;", -1, &stmtUE, 0);
				sqlite3_bind_int(stmtUE, 1, buf);

				sqlite3_step(stmtUE);

				int d = (int)sqlite3_column_int(stmtUE, 0);
				string  name = (char *)sqlite3_column_text(stmtUE, 1);

				cout << time_s << "|    |" << name << endl;

				sqlite3_reset(stmtUE);
				sqlite3_finalize(stmtUE);

			}
			sqlite3_reset(stmtU);
			sqlite3_finalize(stmtU);

		}

		void _sqlResult()
		{
			_sqlResParam("Arrive on time:");
			_sqlResParam("Laters:");
			_sqlResParam("Missing:");

		}


		void _sqlSelect()
		{
			checkCreate();
			int rc = 0;
			rc = sqlite3_prepare16(db, L"SELECT * FROM 'Users'", -1, &stmtU, 0);
			//sqlite3_step(stmt);

			int dat = -1;
			char *buf;
			while (sqlite3_step(stmtU) == SQLITE_ROW)
			{
				dat = (int)sqlite3_column_int(stmtU, 0);
				buf = (char*)sqlite3_column_text(stmtU, 1);

			cout << dat << "|" << buf << endl;

			}
			sqlite3_reset(stmtU);
			sqlite3_finalize(stmtU);
		}

		int get_id(string value){

			int rc = 0;
			rc = sqlite3_prepare16(db, L"SELECT id FROM 'Users' where ini=?", -1, &stmtU, 0);
			const char * val = value.c_str();
			sqlite3_bind_text(stmtU, 1, val, -1, 0);
			int dat = -1;
			sqlite3_step(stmtU);

				dat = (int)sqlite3_column_int(stmtU, 0);


			sqlite3_reset(stmtU);
			sqlite3_finalize(stmtU);
			if (dat == 0) {
				err++;
				return 14;
			}
			return dat;
		}


		string true_name(string s) {

			int f;
			int i = 0;
			string res = "";
			while (i<3) {
							
				f = s.find(' ');
				if (f == -1) {
					res += s;
					res += " ";
					break;
				}
				if (f > 4){
														
					res += s.substr(0,f);
					res += " ";
					i++;
				}
				s = s.substr(f + 1, s.length());
			}
			res = res.substr(0, res.length() - 1);
			return res;
		}

		int _sql_new() {

			int stat;
			checkCreate();




				const wchar_t* _SQLquery = L"DELETE FROM 'ListUs'";
				sqlite3_prepare16(db, _SQLquery, -1, &stmtU, 0);



				sqlite3_step(stmtU);
				sqlite3_reset(stmtU);
				sqlite3_finalize(stmtU);





			for (int t = PEOPLE - 1; t >= 0; t--) {

				string time = ary[t][1];

				int f = time.find(":");
				string hour = time.substr(0, f);
				string minutes = time.substr(f + 1, time.length() - 1);
				const char * h = hour.c_str();
				const char * m = minutes.c_str();
				float tim = atof(h) + atof(m) / 100;


				
				int nameid = get_id(true_name(ary[t][2]));

				if (err > MAX_ERR_REGOGN) {

					
					cout << endl << endl << endl << "Error recognition. Please try another photo"<< endl << endl << endl << endl;
					flag = 1;
					return 1;
					

				}
				else {
					flag = 0;
				}
				if (ary[t][3] == "") {

					stat = 0;

				}
				else {
					stat = 1;


				}


					const wchar_t* _SQLquery = L"INSERT INTO 'ListUs'(id,nameid,time,turnout) VALUES(?,?,?,?);";
					sqlite3_prepare16(db, _SQLquery, -1, &stmtU, 0);

					sqlite3_bind_int(stmtU, 2, nameid);
					sqlite3_bind_double(stmtU, 3, tim);
					sqlite3_bind_int(stmtU, 4, stat);


					sqlite3_step(stmtU);
					sqlite3_reset(stmtU);
					sqlite3_finalize(stmtU);

			}
			return 0;

		}

		
};


class listDetector {

protected:
	vector<vector<Point> > contours;
	vector<Point2d>  contourx;
	vector<vector<Point> > contours_simp;
	vector<Vec4i> hierarchy;
	Mat img_w;
	string size;

public:

	


	char * gettext(const char* name) {

		tesseract::TessBaseAPI tessApi;
		tessApi.Init(NULL, "rus");
		if (1) {
			PIX *pix = pixRead(name);
			tessApi.SetImage(pix);
			char *text = tessApi.GetUTF8Text();
			
			return text;
		}
	}


	int getMaxContourId() {

		int maxContourId = 0;
		for (int o = 0; o < contours.size() - 1; o++) {

			if (contourArea(contours[maxContourId]) < contourArea(contours[o + 1]))
				maxContourId = o + 1;
		}
		return maxContourId;
	}


	int getTransform(string name) {

		Mat img = imread(name, 1);
		if (img.empty()) {

			cout << "Photo not found" << endl;
			return 2;
		}
		else {
			Mat imgray;
			Mat canny;
			

			if (img.cols < 500 && img.rows < 500) {
				if (img.rows > img.rows)
					resize(img, img, Size(img.cols * 680 / img.rows, 680));
				else
					resize(img, img, Size(680, img.rows * 680 / img.cols));
				size = "small";
			}
			else {
				if (img.rows > img.rows)
					resize(img, img, Size(img.cols * 900 / img.rows, 900));
				else
					resize(img, img, Size(900, img.rows * 900 / img.cols));
				
				if (img.cols < 800 && img.rows < 800) {
					size = "medium";
				}
				else {
					size = "big";
				}

			}
			
			

			cvtColor(img, imgray, COLOR_BGR2GRAY);
			Canny(imgray, canny, 50, 150);

			findContours(canny, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));

			int N = 1;
			N = min(N, int(contours.size()));

			if (N == 1) {

				int maxContourId = getMaxContourId();
							   
				approxPolyDP(Mat(contours[maxContourId]), contourx,
					arcLength(Mat(contours[maxContourId]), true)*0.02, true);

				if (contourx.size() == 4)
				{
					/*circle(img, contourx[0], 4, Scalar(255,0,0), -1, 8, 0);
					circle(img, contourx[1], 4, Scalar(255,0,0), -1, 8, 0);
					circle(img, contourx[2], 4, Scalar(255,0,0), -1, 8, 0);
					circle(img, contourx[3], 4, Scalar(255,0,0), -1, 8, 0);*/
					//imshow("m",img);
					//waitKey(0);

					Point lt, rt, lb, rb;

					int masx[4] = { contourx[0].x,contourx[1].x,contourx[2].x,contourx[3].x };
					int masidx[4] = { 0,1,2,3 };
					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < 3; j++) {
							if (masx[j] > masx[j + 1]) {
								int b = masx[j];
								int s = masidx[j];
								masx[j] = masx[j + 1];
								masidx[j] = masidx[j + 1];
								masx[j + 1] = b;
								masidx[j + 1] = s;
							}
						}
					}
					int u = 0; Point sort_cont[4];
					for (int i = 0; i < 4; i++) {

						sort_cont[i] = contourx[masidx[i]];
					}

					if (sort_cont[0].y > sort_cont[1].y) {
						lt = sort_cont[1];
						lb = sort_cont[0];
					}
					else {
						lt = sort_cont[0];
						lb = sort_cont[1];
					}

					if (sort_cont[2].y > sort_cont[3].y) {
						rt = sort_cont[3];
						rb = sort_cont[2];
					}
					else {
						rt = sort_cont[2];
						rb = sort_cont[3];
					}

					Point2f  Noad[4];
					Noad[0].x = lt.x - 7;
					Noad[0].y = lt.y - 7;
					Noad[1].x = lb.x - 7;
					Noad[1].y = lb.y + 7;
					Noad[2].x = rb.x + 7;
					Noad[2].y = rb.y + 7;
					Noad[3].x = rt.x + 7;
					Noad[3].y = rt.y - 7;

					Point2f Quad[4];

					Quad[0] = Point2f(0, 0);
					Quad[1] = Point2f(0, img.rows);
					Quad[2] = Point2f(img.cols, img.rows);
					Quad[3] = Point2f(img.cols, 0);

					Mat mask = Mat::zeros(img.cols, img.rows, CV_8UC1);
					floodFill(mask, Point(0, 0), Scalar(255, 255, 255));

					Mat M = getPerspectiveTransform(Noad, Quad);
					warpPerspective(img, mask, M, Size(img.cols, img.rows));
										
					img_w = mask.clone();
					waitKey(0);
					return 0;

				}
				else {
					flag = 1;
					return 1;

				}

			}
		}
	}

	int im_work(string name) {

		int ret = getTransform(name);

		if (ret != 0) {

			if (ret == 1) {
				cout << "Error recognition. Try use another photo" << endl;
			}
			flag = 1;
			return 1;

		}
		else {


			Mat imgray;
			Mat canny;

			
			cvtColor(img_w, imgray, COLOR_BGR2GRAY);
			Canny(imgray, canny, 50, 150);
			findContours(canny, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));

			int N = 1;
			N = min(N, int(contours.size()));

			if (N == 1) {
				
				int maxContourId = getMaxContourId();
				vector<Rect> boundRect(contours.size());
				boundRect[0] = boundingRect(Mat(contours[maxContourId]));

				Mat Table; Mat cannyTable; Mat Table_c;

				Rect r(boundRect[0].x + 3, boundRect[0].y + 3, boundRect[0].width - 6, boundRect[0].height - 6);
						
				imgray(r).copyTo(Table);
				imgray(r).copyTo(Table_c);
				
				if (Table.cols < 500 || Table.rows < 500) {

					cout << endl << endl << endl << "Error recognition photo. Please try another picture" << endl << endl << endl << endl;
					return 1;
				}
				floodFill(Table, Point(Table.cols - 1, Table.rows - 1), Scalar(0, 0, 0));
				floodFill(Table, Point(1, Table.rows - 1), Scalar(0, 0, 0));
				floodFill(Table, Point(Table.cols - 1, 1), Scalar(0, 0, 0));
				floodFill(Table, Point(1, 1), Scalar(0, 0, 0));
								
				erode(Table, Table, Mat());
				dilate(Table, Table, Mat());
				
				Canny(Table, cannyTable, 50, 150);

			
				

				if (size == "small") {
		
					inRange(Table, Scalar(0, 0, 0), Scalar(110,110, 110), Table);
					Canny(Table, cannyTable, 50, 150);
									
				}
				else {
					resize(Table, Table, Size(Table.cols, Table.cols));
					resize(Table_c, Table_c, Size(Table.cols, Table.cols));
					inRange(Table, Scalar(0, 0, 0), Scalar(95, 95, 95), Table);
					Canny(Table, cannyTable, 50, 150);
					
				}
				findContours(cannyTable, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));

				//addWeighted(Table, 3, Table, -1.5, 0, Table);
				//drawContours(Table, contours, -1, Scalar(0, 0, 255), 2, 8, hierarchy, 2, Point());
				

				int stop = contours.size();
				int track = 0;
				int height = 10;
				int width = 300;
				for (int id = 0; id < stop; id++) {
					
					double Area = 0.0;
					int y = track % COLS;
					int g = track / COLS;
					if (g == PEOPLE-1 && y == COLS)break;
					
					int masxx[4];
					if (contours[id].size() != 0 && contourArea(contours[id]) > 150) {


						Mat Elem;
						boundRect[0] = boundingRect(Mat(contours[id]));
						if (1/*boundRect[0].height >= height * 0.8*/) {

							//if (boundRect[0].height > height)height = boundRect[0].height;
							

							//imshow("dfd", Table_c); waitKey(0);


							Rect re(boundRect[0].x + 3, boundRect[0].y + 3, boundRect[0].width - 6, boundRect[0].height - 6);
							Table_c(re).copyTo(Elem);
							resize(Elem, Elem, Size(Elem.cols * 2, Elem.rows * 2));
							cvtColor(Elem, Elem, COLOR_BGR2RGB);
							
							if (contours[id][0].x > Table.cols*0.5) {
								Mat cannyElem;
								
								vector<vector<Point> > contours_in;
								Canny(Elem, cannyElem, 50, 150);

								findContours(cannyElem, contours_in, hierarchy, RETR_LIST, CHAIN_APPROX_NONE, Point(0, 0));
								for (int l = 0; l < contours_in.size(); l++)
									Area += contourArea(contours_in[l]);
							}
							
							//imshow("canEl", cannyElem); waitKey(0);
							
							//imshow("Elem", Elem); waitKey(0);
							if (size == "small") {
								
								if(boundRect[0].width >= width*0.8)
								addWeighted(Elem, 6, Elem, -4.62, 0, Elem);

								if (boundRect[0].width > width)
									width = boundRect[0].width;

							}
							//imshow("AlemWeighted", Elem); waitKey(0);

							imwrite("Elem.png", Elem);

							

							masxx[y] = contours[id][0].x;

							string rew = gettext("Elem.png");
							rew = rew.substr(0, rew.length() - 1);
							
							if (contours[id][0].x > Table.cols*0.5  && Area < 100)rew = "";
							if (contours[id][0].x > Table.cols*0.5  && Area > 100)rew = "arrive";
						
							
							ary[g][y] = rew;

							if (y == COLS - 1) {

								for (int i = 0; i < COLS; i++) {
									for (int j = 0; j < COLS - 1; j++) {
										if (masxx[j] > masxx[j + 1]) {
											int b = masxx[j];
											string s = ary[g][j];
											masxx[j] = masxx[j + 1];
											ary[g][j] = ary[g][j + 1];
											masxx[j + 1] = b;
											ary[g][j + 1] = s;
										}
									}
								}
								
							}

							track++;
						}
					}

				}




			}

			return 0;
		}


	}

	void show_elem(int i, int j) {

		cout << ary[i][j] << " ";

	}

	void tab_show() {

		for (int a = PEOPLE - 1; a >= 0; a--) {
			for (int b = 0; b < COLS; b++) {

				show_elem(a, b);

			}
			cout << endl;
		}
		
	}

	int getname() {

		string r;
		cout << "Enter image's name: ";
		cin >> r;
		Mat ir = imread(r);

		if (r == "end") {

			flag = 1;
			return 0;
		}
		if (ir.empty()) {

			cout << " ---->Photo don't find. Try again (Enter 'end' to out)" << endl;
			getname();

		}
		else {

			im_work(r);
			return 0;
		}

	}

	void clean_ary() {
		
		for (int i = 0; i < PEOPLE; i++)
		{
			for (int j = 0; j < COLS; j++)
			{
				ary[i][j] = {};
			}

		}
	
	
	}

};

int cont() {

	string answer;
	

	listDetector A;
	A.getname();
	_sql C;
	//A.tab_show();
	//C._sqlSelect();
	
	if (C._sql_new() == 0)
	C._sqlResult();
	
	//C._sqlSelectList();

	A.clean_ary();
	string percent = to_string(100*(1-C.get_count_err()/MAX_ERR_REGOGN));
	percent += "%";
	cout << "Percent Regonition:" << percent << endl;
	percent = "";

	while(1){
	
		
		cout << "Recognize also photo? (y/n)";
		cin >> answer;

		if (answer == "y" || answer == "yes") { cont(); break; }
		if (answer == "n" || answer == "no")
			break;

	

	}

	return 0;

}