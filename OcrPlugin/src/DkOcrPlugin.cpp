/*******************************************************************************************************
 DkOcrPlugin.cpp

 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances

 Copyright (C) 2015 #YOUR_NAME

 This file is part of nomacs.

 nomacs is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 nomacs is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *******************************************************************************************************/

#include "DkOcrPlugin.h"

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QAction>
#pragma warning(pop)		// no warnings from includes - end

 /*******************************************************************************************************
  * PLUGIN_CLASS_NAME	- enter the plugin class name (e.g. DkPageExtractionPlugin)
  * #YOUR_NAME			- your name/pseudonym whatever
  * #DATE				- today...
  * #DESCRIPTION		- describe your plugin in one sentence
  * #MENU_NAME			- a user friendly name (e.g. Flip Image)
  * #MENU_STATUS_TIPP	- status tip of your plugin
  * #RUN_ID_1			- generate an ID using: GUID without hyphens generated at http://www.guidgenerator.com/
  * ID_ACTION1			- your action name (e.g. id_flip_horizontally)
  * #ACTION_NAME1		- your action name (e.g. Flip Horizotally - user friendly!)
  * #ACTION_TIPP1		- your action status tip (e.g. Flips an image horizontally - user friendly!)
  *******************************************************************************************************/

#include <iostream>
//#include <QLibrary>
#include "DkOcr.h"
#include <DkBaseViewPort.h>

namespace nmc {

	/**
	*	Constructor
	**/
	DkOcrPlugin::DkOcrPlugin(QObject* parent) : QObject(parent) {


		// Create Settings Toolbar

		auto* mainWindow = getMainWidnow();
		mDockWidgetSettings = new QDockWidget(tr("Ocr Plugin Settings"), mainWindow);
		mainWindow->addDockWidget(Qt::RightDockWidgetArea, mDockWidgetSettings);
		
		te_resultText = new QTextEdit();
		//te_resultText->minimumHeight(100);

		QPushButton* btn_runocr = new QPushButton(tr("Run Ocr"));
		QPushButton* btn_copytoclipboard = new QPushButton(tr("Copy to Clipboard"));
		QPushButton* btn_sendtoeditor = new QPushButton(tr("Open in Editor"));

		connect(btn_runocr, &QPushButton::pressed, [&]()
		{
			qDebug("run ocr pressed");
			mActions[ACTION_TESTRUN]->trigger();
		});

		connect(btn_copytoclipboard, &QPushButton::pressed, [&]()
		{
			qDebug("copy to clip pressed");

			QApplication::clipboard()->setText(te_resultText->toPlainText());
		});

		connect(btn_sendtoeditor, &QPushButton::pressed, [&]()
		{
			qDebug("open in editor pressed");
			QString filename = GetRandomString()+".txt";
			auto saveloc = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/" + filename;
			qDebug("temp file location %s", saveloc);

			QFile outputFile(saveloc);
			outputFile.open(QIODevice::WriteOnly);

			if (!outputFile.isOpen()) {
				qDebug() << "Error, unable to open" << saveloc;
				return;
			}

			QTextStream outStream(&outputFile);
			outStream << te_resultText->toPlainText();
			outputFile.close();

			QDesktopServices::openUrl(QUrl("file:///"+saveloc));
		});

		QVBoxLayout* layout = new QVBoxLayout();
		layout->addWidget(te_resultText);

		QHBoxLayout* btn_layout = new QHBoxLayout();
		btn_layout->addWidget(btn_runocr);
		btn_layout->addWidget(btn_copytoclipboard);
		btn_layout->addWidget(btn_sendtoeditor);
		layout->addLayout(btn_layout);

		mDockWidgetSettings->setLayout(layout);
		QGroupBox* widget = new QGroupBox();
		widget->setLayout(layout);
		mDockWidgetSettings->setWidget(widget);

		// create run IDs
		QVector<QString> runIds;
		runIds.resize(id_end);
		runIds[ACTION_TESTRUN] = "OCR_PLUGIN_TEST_RUN";
		mRunIDs = runIds.toList();

		// create menu actions
		QVector<QString> menuNames;
		menuNames.resize(id_end);

		menuNames[ACTION_TESTRUN] = tr("Testrun");
		mMenuNames = menuNames.toList();

		// create menu status tips
		QVector<QString> statusTips;
		statusTips.resize(id_end);

		statusTips[ACTION_TESTRUN] = tr("#ACTION_TIPP2");
		mMenuStatusTips = statusTips.toList();


		//		

			/*QLibrary libTesseract("E:/dev/tesseract/build_x86/bin/Debug/tesseract305d.dll");
			libTesseract.load();
			if(!libTesseract.isLoaded())
			{
				std::cout << "could not load lib Tesseract" << std::endl;
				exit(1);
			}

			QLibrary libLept("E:/dev/tesseract/build_x86/bin/Debug/liblept171.dll");
			libLept.load();
			if (!libLept.isLoaded())
			{
				std::cout << "could not load lib lib Leptonica" << std::endl;
				exit(1);
			}*/
	}
	/**
	*	Destructor
	**/
	DkOcrPlugin::~DkOcrPlugin() {
	}


	/**
	* Returns unique ID for the generated dll
	**/
	QString DkOcrPlugin::pluginID() const {

		return PLUGIN_ID;
	};


	/**
	* Returns plugin name
	* @param plugin ID
	**/
	QString DkOcrPlugin::pluginName() const {

		return tr("Ocr Plugin");
	};

	/**
	* Returns long description for every ID
	* @param plugin ID
	**/
	QString DkOcrPlugin::pluginDescription() const {

		return "<b>Created by:</b> Dominik Schörkhuber <br><b>Modified:</b>9.10.2015<br><b>Description:</b> #DESCRIPTION.";
	};

	/**
	* Returns descriptive iamge for every ID
	* @param plugin ID
	**/
	QImage DkOcrPlugin::pluginDescriptionImage() const {

		return QImage(":/#PLUGIN_NAME/img/your-image.png");
	};

	/**
	* Returns plugin version for every ID
	* @param plugin ID
	**/
	QString DkOcrPlugin::pluginVersion() const {

		return PLUGIN_VERSION;
	};

	/**
	* Returns unique IDs for every plugin in this dll
	**/
	QStringList DkOcrPlugin::runID() const {

		//GUID without hyphens generated at http://www.guidgenerator.com/
		return QStringList() << "4acb88c461024cb080ae5cd15d0ef0ec";
	};

	/**
	* Returns plugin name for every ID
	* @param plugin ID
	**/
	QString DkOcrPlugin::pluginMenuName(const QString &runID) const {

		return tr("Run Ocr");
	};

	/**
	* Returns short description for status tip for every ID
	* @param plugin ID
	**/
	QString DkOcrPlugin::pluginStatusTip(const QString &runID) const {

		return tr("#MENU_STATUS_TIP");
	};

	QList<QAction*> DkOcrPlugin::createActions(QWidget* parent)  {

		if (mActions.empty()) {			
			
			auto* ca = new QAction(mMenuNames[ACTION_TESTRUN], this);
			ca->setObjectName(mMenuNames[ACTION_TESTRUN]);
			ca->setStatusTip(mMenuStatusTips[ACTION_TESTRUN]);
			ca->setData(mRunIDs[ACTION_TESTRUN]);	// runID needed for calling function runPlugin()
			mActions.append(ca);

			// additional action
			mActions.append(mDockWidgetSettings->toggleViewAction());
		}

		return mActions;
	}

	QList<QAction*> DkOcrPlugin::pluginActions() const {
		return mActions;
	}

	QSharedPointer<DkImageContainer> DkOcrPlugin::runPlugin(const QString& runID, QSharedPointer<DkImageContainer> imgC) const
	{
		if (runID == mRunIDs[ACTION_TESTRUN]) {

			qInfo("testrun action");

			auto img = imgC->image();
			auto text = Ocr::testOcr(img);
			te_resultText->setText(text);
			imgC->setImage(img);
		}

		// wrong runID? - do nothing
		return imgC;
	}

	QString DkOcrPlugin::GetRandomString() const
	{
		const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
		const int randomStringLength = 12; // assuming you want random strings of 12 characters

		QString randomString;
		for (int i = 0; i<randomStringLength; ++i)
		{
			int index = qrand() % possibleCharacters.length();
			QChar nextChar = possibleCharacters.at(index);
			randomString.append(nextChar);
		}
		return randomString;
	}
}