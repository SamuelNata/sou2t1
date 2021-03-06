/* Código retirado do seguinte blog: http://www.ics.com/blog/integrating-custom-widget-qt-designer
 * Data: 02/04/2016
 */

#include "LED.h"
#include "LEDPlugin.h"

#include <QtPlugin>

LEDPlugin::
LEDPlugin(QObject* parent) :
	QObject(parent),
	initialized(false)
{
}

QString LEDPlugin::
name() const
{
	return "LED";
}

QString LEDPlugin::
group() const
{
	return tr("ICS Custom Widgets");
}

QString LEDPlugin::
toolTip() const
{
	return tr("An LED");
}

QString LEDPlugin::
whatsThis() const
{
	return tr("An LED");
}

QString LEDPlugin::
includeFile() const
{
	return "LED.h";
}

QIcon LEDPlugin::
icon() const
{
	return QIcon();
}

bool LEDPlugin::
isContainer() const
{
	return false;
}

QWidget * LEDPlugin::
createWidget(QWidget *parent)
{
	return new LED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(ledplugin, LEDPlugin)
#endif
