/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "formextractor.h"

#include <QWebElement>

FormExtractor::FormExtractor(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);
    ui.webView->setUrl(QUrl("qrc:/form.html"));
    connect(ui.webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(populateJavaScriptWindowObject()));
    resize(300, 300);
}

FormExtractor::~FormExtractor()
{
}

int FormExtractor::loadUrl(QString fn)
{
    int iRetval = 0;

    //ui.webView->setUrl(QUrl("qrc:/form.html"));
    QString url = "http://www.joeschedule.com";
    //ui.webView->load(url);
    ui.webView->load(QUrl::fromLocalFile(fn));

    connect(ui.webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(populateJavaScriptWindowObject()));
    resize(300, 300);

    return iRetval;

}

void list(QWebElementCollection list)
{
    foreach(QWebElement elem,  list)
    {
        //qDebug() << elem.evaluateJavaScript("this.name").toString();
        //qDebug() << elem.evaluateJavaScript("this.value").toString();
        QString row = QString("%1 : %2").arg(
                    elem.evaluateJavaScript("this.name").toString()
                   ,elem.evaluateJavaScript("this.value").toString());

        qDebug() << row;
    }
}

void FormExtractor::listFormFields()
{
    QWebFrame *frame = ui.webView->page()->mainFrame();

    /*
    QWebElement vvv = frame->findFirstElement("#fld");
    qDebug() << vvv.evaluateJavaScript("this.value").toString();
    QWebElementCollection vvvs = frame->findAllElements("#fld");
    qDebug() << vvvs[0].evaluateJavaScript("this.value").toString();
    qDebug() << vvvs[1].evaluateJavaScript("this.value").toString();
*/
//    QWebElement firstName = frame->findFirstElement("#firstname");
//    qDebug() << "_____ " <<  firstName.toInnerXml() << "\n";

list( frame->findAllElements("#fld"));

    QWebElementCollection list = frame->findAllElements("#fld");
    foreach(QWebElement elem,  list)
    {
        qDebug() << "\t { rf\n";
         qDebug() << elem.evaluateJavaScript("this.value").toString();

        //firstName.evaluateJavaScript("this.value").toString());

        qDebug() << elem.toInnerXml();
        qDebug() << elem.toOuterXml();
        qDebug() << "\t } rf\n";

    }

   // QString sss = firstName.evaluateJavaScript("this.value").toString();
    //qDebug() << sss;

     //firstName.setAttribute("value", "fu buddy");

     //firstName.setAttribute("value", "fu buddy");

    //QString sel = "input[id=\"login\"]"";
    //frame->findAllElements(sel);

    //QWebElement lastName = frame->findFirstElement("#lastname");
}

void FormExtractor::submit()
{
    this->listFormFields();

    QWebFrame *frame = ui.webView->page()->mainFrame();

    QWebElement firstName = frame->findFirstElement("#firstname");
    QWebElement lastName = frame->findFirstElement("#lastname");
    QWebElement maleGender = frame->findFirstElement("#genderMale");
    QWebElement femaleGender = frame->findFirstElement("#genderFemale");
    QWebElement updates = frame->findFirstElement("#updates");

    ui.firstNameEdit->setText(firstName.evaluateJavaScript("this.value").toString());
    ui.lastNameEdit->setText(lastName.evaluateJavaScript("this.value").toString());

    if (maleGender.evaluateJavaScript("this.checked").toBool())
        ui.genderEdit->setText(maleGender.evaluateJavaScript("this.value").toString());
    else if (femaleGender.evaluateJavaScript("this.checked").toBool())
        ui.genderEdit->setText(femaleGender.evaluateJavaScript("this.value").toString());

    if (updates.evaluateJavaScript("this.checked").toBool())
        ui.updatesEdit->setText("Yes");
    else
        ui.updatesEdit->setText("No");
}

void FormExtractor::populateJavaScriptWindowObject()
{
    ui.webView->page()->mainFrame()->addToJavaScriptWindowObject("formExtractor", this);
}
