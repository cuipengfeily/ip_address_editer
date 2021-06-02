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

#include <qlineedit.h>
#include <qwidget.h>
#include <qlabel.h>
#include <QHBoxLayout>
#include <QRegExpValidator>
#include <QEvent>
#include <QKeyEvent>
#include <QPainter>

#include "ip_edit.h"

namespace tool
{
    void DotWidget::paintEvent(QPaintEvent*)
    {
        paint = new QPainter;
        paint->begin(this);
        paint->setBrush(QBrush(Qt::black, Qt::SolidPattern));
        
        // circle center
        QPointF center(this->height() / 2, this->height() / 2);

        // draw circle
        int radius = this->height() / 9;
        if (radius < 1)
            radius = 1;
        paint->drawEllipse(center, radius, radius);
        paint->end();
    }

    DotWidget::DotWidget(QWidget*parent) :QWidget(parent)
    {
        //this->setStyleSheet("background-color: rgb(0, 0, 255);");
    }

    IpAddressLineEdit::IpAddressLineEdit(QWidget *parent /* = 0 */, QString ip) :QLineEdit(parent)
    {
        QString info;
        QString sty = "border-width:0;border-style:outset;";// no border
        QStringList default_ip = ip.split(".");

        // ip address content
        QRegExp rx("[0-9]{0,4}[\.]{0,4}");// input data regexp
        for (int i = 0; i<4; i++)
        {
            m_LineEdit[i] = new QLineEdit(this);
            m_LineEdit[i]->setObjectName(info.sprintf("LineEdit %d", i));
            m_LineEdit[i]->setMaxLength(4);
            m_LineEdit[i]->setAlignment(Qt::AlignCenter);
            QSizePolicy pol = m_LineEdit[i]->sizePolicy();
            pol.setVerticalPolicy(QSizePolicy::Expanding);
            m_LineEdit[i]->setSizePolicy(pol);
            m_LineEdit[i]->setAttribute(Qt::WA_InputMethodEnabled, false);
            m_LineEdit[i]->setStyleSheet(sty);
            m_LineEdit[i]->installEventFilter(this);

            m_LineEdit[i]->setValidator(new QRegExpValidator(rx, this));
            m_LineEdit[i]->setText(default_ip[i]);

            connect(m_LineEdit[i], SIGNAL(textEdited(const QString&)), this, SLOT(lineEditTextChanged(const QString&)));
            connect(m_LineEdit[i], SIGNAL(editingFinished()), this, SLOT(lineEditFinished()));

        }

        // three dots
        for (int i = 0; i<3; i++)
        {
            m_Dot[i] = new DotWidget(this);
            m_Dot[i]->setMinimumWidth(20);
        }

        // layout: id.ip.ip.ip, for example 192.168.1.1
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setSpacing(0);
        layout->setContentsMargins(1, 1, 1, 1);
        for (int i = 0; i<3; i++)
        {
            layout->addWidget(m_LineEdit[i]);
            layout->addWidget(m_Dot[i]);
        }
        layout->addWidget(m_LineEdit[3]);
        setLayout(layout);

    }

    IpAddressLineEdit::~IpAddressLineEdit()
    {

    }

    void IpAddressLineEdit::keyPressEvent(QKeyEvent* e)
    {
        QWidget::keyPressEvent(e);
    }

    QString IpAddressLineEdit::getIpAddress()
    {
        QString ipAddress;
        for (int i = 0; i<3; i++)
        {
            if (m_LineEdit[i]->text().isEmpty())
            {
                ipAddress.clear();
                return ipAddress;
            }
            ipAddress += m_LineEdit[i]->text() + QString('.');
        }
        ipAddress += m_LineEdit[3]->text();

        QStringList ips = ipAddress.split(".");
        if (ips.size() != 4)
            ipAddress.clear();
        else
        {
            int ip_value[4] = { -1,-1,-1,-1 };
            bool ok = false;
            for (int i = 0; i < ips.size(); i++)
            {
                ip_value[i] = ips[i].toInt(&ok);
                if ((!ok) || (ip_value[i] > 255) || (ip_value[i] < 0))
                {
                    ipAddress.clear();
                    break;
                }
            }
        }
        return ipAddress;
    }

    void IpAddressLineEdit::setIpAddress(QString ip)
    {
        QStringList default_ip = ip.split(".");
        for (int i = 0; i < 4; i++)
        {
            m_LineEdit[i]->setText(default_ip[i]);
        }
    }

    void IpAddressLineEdit::lineEditFinished()
    {
        QLineEdit* edit = (QLineEdit*)sender();
        if (edit->text().size() == 0)
        {
            edit->setText("0");
        }
    }

    void IpAddressLineEdit::lineEditTextChanged(const QString &text)
    {
        QLineEdit* edit = (QLineEdit*)sender();
        int nowBox = -1;
        for (int i = 0; i<4; i++)
        {
            if (edit == m_LineEdit[i])
                nowBox = i;
        }
        if (-1 == nowBox)
            return;

        QString content = text;

        // if input is '.' the next editer get the focus, and selected automatically
        if (text.endsWith("."))
        {
            content.remove('.');
            edit->setText(content);
            if (nowBox < 3)
            {
                this->m_LineEdit[nowBox + 1]->setFocus();
                this->m_LineEdit[nowBox + 1]->selectAll();
            }
            return;
        }

        // remove 0 at left
        int size = text.size();
        if ((size > 1) && text.startsWith('0'))
        {
            content = text.mid(1, -1);
            edit->setText(content);
            return;
        }

        // length is less than 3
        if (text.size() == 4)
        {
            content = text.mid(0, 3);
            edit->setText(content);
            return;
        }
        else if (text.size() == 3)// must start with '1' or '2'
        {
            if ((text[0] != '1') && (text[0] != '2'))
            {
                content = text.mid(1, -1);
                edit->setText(content);
                return;
            }
        }
        else if (text.size() == 2)
        {
            ;
        }
        else if (text.size() == 1)
        {
            ;
        }
    }

    bool IpAddressLineEdit::eventFilter(QObject *object, QEvent *event)
    {
        // which editor the sigal comes from
        int nowBox = -1;
        for (int i = 0; i<4; i++)
        {
            if (object == m_LineEdit[i])
                nowBox = i;
        }

        if (-1 == nowBox)
            return QWidget::eventFilter(object, event);

        QLineEdit *spinBox = dynamic_cast<QLineEdit*>(object);

        if ((QEvent::KeyPress == event->type()))
        {
            int pos = spinBox->cursorPosition();
            QKeyEvent* kev = dynamic_cast<QKeyEvent*>(event);

            // move to left editor
            if ((0 == pos) && (nowBox > 0))
            {
                if ((Qt::Key_Left == kev->key()))
                    m_LineEdit[nowBox - 1]->setFocus();
                else if (Qt::Key_Backspace == kev->key())
                {
                    m_LineEdit[nowBox - 1]->setFocus();
                    return true;
                }
            }

            // move to right editor
            if (((pos) == spinBox->text().size()) && (nowBox < 3) && (Qt::Key_Right == kev->key()))
            {
                m_LineEdit[nowBox + 1]->setFocus();
            }
        }

        return QLineEdit::eventFilter(object, event);
    }

    // not used
    IpAddressEditer::IpAddressEditer(QWidget *parent /* = 0 */, QString ip) : QLineEdit(parent)
    {
        this->editer_ = new IpAddressLineEdit(parent, ip);
        QHBoxLayout* background_layout = new QHBoxLayout(this);
        background_layout->addWidget(this->editer_);
        background_layout->setSpacing(0);
        background_layout->setContentsMargins(1, 0, 1, 0);
        this->setLayout(background_layout);
    }

    IpAddressEditer::~IpAddressEditer()
    {

    }

    QString IpAddressEditer::getIpAddressString()
    {
        return editer_->getIpAddress();
    }

    void IpAddressEditer::setIpAddressString(QString ip)
    {
        return editer_->setIpAddress(ip);
    }

}