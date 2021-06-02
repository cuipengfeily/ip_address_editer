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

#ifndef IP_EDIT_H
#define IP_EDIT_H

#include <qlineedit.h>
#include <qlabel.h>

namespace tool
{
    class DotWidget :public QWidget
    {
    public:
        DotWidget(QWidget *parent = 0);
    private:
        void paintEvent(QPaintEvent*);
        QPainter *paint;
    };

    class IpAddressLineEdit : public QLineEdit
    {
        Q_OBJECT
    private:

        // ip content by value
        QLineEdit *m_LineEdit[4];

        // for dot(seperator)
        DotWidget *m_Dot[3];

    protected:

        // move to the left or the right editer
        bool eventFilter(QObject *object, QEvent *event);

        // not used yet
        void keyPressEvent(QKeyEvent* e);

    public:

        // default is 192.168.1.1
        IpAddressLineEdit(QWidget *parent = 0, QString ip = "192.168.1.1");
        ~IpAddressLineEdit();

        // get ip address from the editer
        QString getIpAddress();
        void setIpAddress(QString ip = "192.168.1.1");

        private slots :

        // if the editer is finished, check the value is empty or not ,if empty set to 0
        void lineEditFinished();

        // if the editer content is changed, check the current calue and modify it if necessary
        void lineEditTextChanged(const QString &);

    };


    // not used
    class IpAddressEditer : public QLineEdit
    {
        Q_OBJECT
    private:
        IpAddressLineEdit *editer_;

    public:
        IpAddressEditer(QWidget *parent = 0, QString ip = "192.168.10.108");
        ~IpAddressEditer();
        QString getIpAddressString();
        void setIpAddressString(QString ip = "192.168.10.108");

    };

}

#endif // IP_EDIT_H
