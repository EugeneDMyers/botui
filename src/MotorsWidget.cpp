#include "MotorsWidget.h"
#include "ui_MotorsWidget.h"
#include "MenuBar.h"
#include "RootController.h"
#include "StatusBar.h"
#include "Device.h"
#include "NumpadDialog.h"
#include "kovan/kovan.h"

#include <QDebug>

MotorsWidget::MotorsWidget(Device *device, QWidget *parent)
	: StandardWidget(device, parent),
	ui(new Ui::MotorsWidget),
	m_provider(new NumpadDialog(QString()))
{
	ui->setupUi(this);
	performStandardSetup(tr("Motors"));
	
	ui->number->setInputProvider(m_provider);
	
	connect(ui->dial, SIGNAL(valueChanged(double)), SLOT(valueChanged(double)));
	connect(ui->_0, SIGNAL(clicked()), SLOT(activeChanged()));
	connect(ui->_1, SIGNAL(clicked()), SLOT(activeChanged()));
	connect(ui->_2, SIGNAL(clicked()), SLOT(activeChanged()));
	connect(ui->_3, SIGNAL(clicked()), SLOT(activeChanged()));
	connect(ui->number, SIGNAL(textEdited(QString)), SLOT(manualEntry(QString)));
	
	ui->dial->setMinimumValue(-100);
	ui->dial->setMaximumValue(100);
	ui->dial->setValue(0);
	
	ui->dial->setLabel(0);
	ui->_0->setEnabled(false);
	ao();
}

MotorsWidget::~MotorsWidget()
{
	ao();
	delete ui;
	delete m_provider;
}

void MotorsWidget::valueChanged(const double &value)
{
	ui->number->setText(QString::number((int)ui->dial->value()));
	motor(ui->dial->label(), value);
}

void MotorsWidget::activeChanged()
{
	QObject *from = sender();
	if(!from) return;
	
	off(ui->dial->label());
	
	quint16 label = 0xFFFF;
	
	if(from == ui->_0) label = 0;
	else if(from == ui->_1) label = 1;
	else if(from == ui->_2) label = 2;
	else if(from == ui->_3) label = 3;
	
	ui->_0->setEnabled(from != ui->_0);
	ui->_1->setEnabled(from != ui->_1);
	ui->_2->setEnabled(from != ui->_2);
	ui->_3->setEnabled(from != ui->_3);
	
	off(label);
	ui->dial->setLabel(label);
	ui->dial->setValue(0);
}

void MotorsWidget::manualEntry(const QString &text)
{
	ui->dial->setValue(text.toInt());
}