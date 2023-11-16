#include "wvs_main_window.h"
#include <QApplication>
#include <QStyleFactory>

/*
* Reza Adhitya Saputra
* reza.adhitya.saputra@gmail.com
*
*/

int main(int argc, char* argv[])
{
	//system("./concorde d493.tsp");

	QApplication a(argc, argv);
	QApplication::setStyle(QStyleFactory::create("fusion"));
	QPalette p;
	p = qApp->palette();
	p.setColor(QPalette::Button, QColor(200, 200, 200));
	qApp->setPalette(p);

	wvs_main_window w;
	w.show();

	return a.exec();
}
