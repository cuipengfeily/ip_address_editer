/*
* Copyright (c) 2021 PengfeiCui.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtWidgets/QApplication>
#include <QDialog>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include "ip_edit.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QMainWindow* w = new QMainWindow(NULL);

    QWidget* central = new QWidget(w);
    QGridLayout *total = new QGridLayout(w);

    for (int i = 0; i < 10; i++)
    {
        // 
        tool::IpAddressLineEdit *edit = new tool::IpAddressLineEdit(central);
        total->addWidget(edit, i, 0);
    }
    
    tool::DotWidget* dot = new tool::DotWidget();
    total->addWidget(dot, 10, 0);

    central->setLayout(total);
    w->setCentralWidget(central);
    w->show();
    return a.exec();
}
