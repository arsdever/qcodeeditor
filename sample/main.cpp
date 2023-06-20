#include <QApplication>

#include "qcodeeditor.hpp"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

    QCodeEditor editor;
    editor.show();

	return app.exec();
}
