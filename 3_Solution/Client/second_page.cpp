#include "second_page.h"
#include "mainwindow.h"
#include "ui_second_page.h"

#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/qnetworkreply.h>
#include <QMessageBox>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Second_Page::Second_Page(QWidget *parent, QTcpSocket* socket) :
    QDialog(parent),
    ui(new Ui::Second_Page)
{
    this->socket = socket;
    ui->setupUi(this);

    ui->textEdit->setReadOnly(true);  // Make the textEdit widget read-only
    // Make the initial API request
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Second_Page::onNetworkReply);

    QNetworkRequest request(initialApiUrl);
    manager->get(request);



    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(listen_for_alerts()));
    timer->start(1000);
}

Second_Page::~Second_Page()
{
    delete ui;
}

void Second_Page::on_pushButton_clicked()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Second_Page::onNetworkReply);

    QNetworkRequest request(initialApiUrl);
    manager->get(request);
}

void Second_Page::on_pushButton_2_clicked()
{
    this->close();
    mw=new MainWindow;
    mw->show();
}


void Second_Page::on_pushButton_3_clicked()
{
    QString tara = ui->lineEdit->text();
    QString dezastru = ui->lineEdit_2->text();
    QString status = ui->lineEdit_3->text();

    QString url;

    disasterVector  <<DisasterInfo{4611,"Flood"}
                    <<DisasterInfo{4628,"Earthquake"}
                    <<DisasterInfo{4672,"Drought"}
                    <<DisasterInfo{4930,"Heat Wave"}
                    <<DisasterInfo{4628,"Wild Fire"}
                    <<DisasterInfo{4642,"Epidemic"}
                    <<DisasterInfo{4656,"Severe Local Storm"}
                    <<DisasterInfo{4728,"Land Slide"}
                    <<DisasterInfo{4618,"Cyclone"}
                    <<DisasterInfo{4615,"Volcano"}
                    <<DisasterInfo{4687,"Tsunami"}
                    <<DisasterInfo{4764,"Snow Avalanche"}
                    <<DisasterInfo{5706,"Fire"}
                    <<DisasterInfo{4620,"Tehnological Disaster"}
                    <<DisasterInfo{4653,"Cold Wave"}
                    <<DisasterInfo{4719,"Extratropical Cyclone"}
                    <<DisasterInfo{5255,"Insect Infestation"}
                    <<DisasterInfo{5549,"Storm Surge"}
                    <<DisasterInfo{5402,"Other"};

    countryVector<< CountryInfo{13, "Afghanistan"}
                << CountryInfo{15, "Albania"}
                << CountryInfo{16, "Algeria"}
                << CountryInfo{18, "Andorra"}
                << CountryInfo{19, "Angola"}
                << CountryInfo{21, "Antigua and Barbuda"}
                << CountryInfo{22, "Argentina"}
                << CountryInfo{23, "Armenia"}
                << CountryInfo{25, "Australia"}
                << CountryInfo{26, "Austria"}
                << CountryInfo{27, "Azerbaijan"}
                << CountryInfo{29, "Bahamas"}
                << CountryInfo{30, "Bahrain"}
                << CountryInfo{31, "Bangladesh"}
                << CountryInfo{32, "Barbados"}
                << CountryInfo{33, "Belarus"}
                << CountryInfo{34, "Belgium"}
                << CountryInfo{35, "Belize"}
                << CountryInfo{36, "Benin"}
                << CountryInfo{38, "Bhutan"}
                << CountryInfo{39, "Bolivia"}
                << CountryInfo{40, "Bosnia and Herzegovina"}
                << CountryInfo{41, "Botswana"}
                << CountryInfo{42, "Brazil"}
                << CountryInfo{44, "Brunei"}
                << CountryInfo{45, "Bulgaria"}
                << CountryInfo{46, "Burkina Faso"}
                << CountryInfo{47, "Burundi"}
                << CountryInfo{48, "Cambodia"}
                << CountryInfo{49, "Cameroon"}
                << CountryInfo{49, "Cameroon"}
                << CountryInfo{50, "Canada"}
                << CountryInfo{54, "Central African Republic"}
                << CountryInfo{55, "Chad"}
                << CountryInfo{57, "Chile"}
                << CountryInfo{58, "China"}
                << CountryInfo{64, "Colombia"}
                << CountryInfo{65, "Comoros"}
                << CountryInfo{66, "Congo"}
                << CountryInfo{68, "Costa Rica"}
                << CountryInfo{69, "Côte d'Ivoire"}
                << CountryInfo{70, "Croatia"}
                << CountryInfo{71, "Cuba"}
                << CountryInfo{72, "Cyprus"}
                << CountryInfo{73, "Czehia"}
                << CountryInfo{75, "Democratic Replublic of Congo"}
                << CountryInfo{76, "Denmark"}
                << CountryInfo{77, "Djibouti"}
                << CountryInfo{78, "Dominica"}
                << CountryInfo{79, "Dominican Republic"}
                << CountryInfo{81, "Ecuador"}
                << CountryInfo{82, "Egypt"}
                << CountryInfo{83, "El Salvador"}
                << CountryInfo{84, "Equatorial Guinea"}
                << CountryInfo{85, "Eritrea"}
                << CountryInfo{86, "Estonia"}
                << CountryInfo{87, "Ethiopia"}
                << CountryInfo{90, "Fiji"}
                << CountryInfo{91, "Finland"}
                << CountryInfo{92, "France"}
                << CountryInfo{96, "Gabon"}
                << CountryInfo{98, "Gambia"}
                << CountryInfo{100, "Georgia"}
                << CountryInfo{101, "Germany"}
                << CountryInfo{102, "Ghana"}
                << CountryInfo{104, "Greece"}
                << CountryInfo{106, "Grenada"}
                << CountryInfo{109, "Guatemala"}
                << CountryInfo{110, "Guinea"}
                << CountryInfo{111, "Guinea-Bissau"}
                << CountryInfo{112, "Guyana"}
                << CountryInfo{113, "Haiti"}
                << CountryInfo{115, "Holy See"}
                << CountryInfo{116, "Honduras"}
                << CountryInfo{117, "Hungary"}
                << CountryInfo{118, "Iceland"}
                << CountryInfo{119, "India"}
                << CountryInfo{120, "Indonesia"}
                << CountryInfo{121, "Iran"}
                << CountryInfo{122, "Iraq"}
                << CountryInfo{123, "Ireland"}
                << CountryInfo{125, "Israel"}
                << CountryInfo{126, "Italy"}
                << CountryInfo{127, "Jamaica"}
                << CountryInfo{128, "Japan"}
                << CountryInfo{129, "Jordan"}
                << CountryInfo{130, "Kazakhstan"}
                << CountryInfo{131, "Kenya"}
                << CountryInfo{132, "Kiribati"}
                << CountryInfo{133, "Kuwait"}
                << CountryInfo{134, "Kyrgyzstan"}
                << CountryInfo{135, "Lao"}
                << CountryInfo{136, "Latvia"}
                << CountryInfo{137, "Lebanon"}
                << CountryInfo{138, "Lesotho"}
                << CountryInfo{139, "Liberia"}
                << CountryInfo{140, "Libya"}
                << CountryInfo{141, "Liechtenstein"}
                << CountryInfo{142, "Lithuania"}
                << CountryInfo{143, "Luxembourg"}
                << CountryInfo{144, "Madagascar"}
                << CountryInfo{146, "Malawi"}
                << CountryInfo{147, "Malaysia"}
                << CountryInfo{148, "Maldives"}
                << CountryInfo{149, "Mali"}
                << CountryInfo{150, "Malta"}
                << CountryInfo{156, "Mexico"}
                << CountryInfo{157, "Micronesia"}
                << CountryInfo{158, "Moldova"}
                << CountryInfo{159, "Monaco"}
                << CountryInfo{160, "Mongolia"}
                << CountryInfo{161, "Montenegro"}
                << CountryInfo{163, "Morocco"}
                << CountryInfo{164, "Mozambique"}
                << CountryInfo{165, "Myanmar"}
                << CountryInfo{166, "Namibia"}
                << CountryInfo{167, "Nauru"}
                << CountryInfo{168, "Nepal"}
                << CountryInfo{169, "Netherlands"}
                << CountryInfo{172, "New Zealand"}
                << CountryInfo{173, "Nicaragua"}
                << CountryInfo{174, "Niger"}
                << CountryInfo{175, "Nigeria"}
                << CountryInfo{74, "North Korea"}
                << CountryInfo{229, "North Macedonia"}
                << CountryInfo{179, "Norway"}
                << CountryInfo{181, "Oman"}
                << CountryInfo{182, "Pakistan"}
                << CountryInfo{183, "Palau"}
                << CountryInfo{180, "Palestine"}
                << CountryInfo{184, "Panama"}
                << CountryInfo{185, "Papua New Guinea"}
                << CountryInfo{186, "Paraguay"}
                << CountryInfo{187, "Peru"}
                << CountryInfo{188, "Philippines"}
                << CountryInfo{190, "Poland"}
                << CountryInfo{193, "Qatar"}
                << CountryInfo{196, "Romania"}
                << CountryInfo{197, "Russia"}
                << CountryInfo{198, "Rwanda"}
                << CountryInfo{200, "Saint Kitts and Nevis"}
                << CountryInfo{201, "Saint Lucia"}
                << CountryInfo{203, "Saint Vincent and the Grenadines"}
                << CountryInfo{204, "Samoa"}
                << CountryInfo{205, "San Marino"}
                << CountryInfo{206, "Sao Tome and Principe"}
                << CountryInfo{207, "Saudi Arabia"}
                << CountryInfo{208, "Senegal"}
                << CountryInfo{209, "Serbia"}
                << CountryInfo{210, "Seychelles"}
                << CountryInfo{211, "Sierra Leone"}
                << CountryInfo{212, "Singapore"}
                << CountryInfo{213, "Slovakia"}
                << CountryInfo{214, "Slovenia"}
                << CountryInfo{215, "Solomon Islands"}
                << CountryInfo{216, "Somalia"}
                << CountryInfo{217, "South Africa"}
                << CountryInfo{194, "South Korea"}
                << CountryInfo{8657, "South Sudan"}
                << CountryInfo{218, "Spain"}
                << CountryInfo{219, "Sri Lanka"}
                << CountryInfo{220, "Sudan"}
                << CountryInfo{221, "Suriname"}
                << CountryInfo{224, "Sweden"}
                << CountryInfo{225, "Switzerland"}
                << CountryInfo{226, "Syria"}
                << CountryInfo{227, "Tajikistan"}
                << CountryInfo{244, "Tanzania"}
                << CountryInfo{228, "Thailand"}
                << CountryInfo{230, "Timor-Leste"}
                << CountryInfo{231, "Togo"}
                << CountryInfo{233, "Tonga"}
                << CountryInfo{234, "Trinidad and Tobago"}
                << CountryInfo{235, "Tunisia"}
                << CountryInfo{236, "Turkey"}
                << CountryInfo{237, "Turkmenistan"}
                << CountryInfo{239, "Tuvalu"}
                << CountryInfo{240, "Uganda"}
                << CountryInfo{241, "Ukraine"}
                << CountryInfo{242, "United Arab Emirates"}
                << CountryInfo{243, "UK"}
                << CountryInfo{245, "USA"}
                << CountryInfo{247, "Uruguay"}
                << CountryInfo{248, "Uzbekistan"}
                << CountryInfo{249, "Vanuatu"}
                << CountryInfo{250, "Venezuela"}
                << CountryInfo{251, "Vietnam"}
                << CountryInfo{255, "Yemen"}
                << CountryInfo{256, "Zambia"}
                << CountryInfo{257, "Zimbabwe"};

    for(int i =0;i<190;i++)
        if(tara==countryVector[i].country_name)
            tara=QString::number(countryVector[i].id);

    for(int j =0;j<17;j++)
        if(dezastru==disasterVector[j].disaster_name)
            dezastru=QString::number(disasterVector[j].id);

    if (status.isEmpty() && tara.isEmpty() && dezastru.isEmpty()) {
        // Use initial API URL
        url = initialApiUrl;
    } else {
        // Use search formula API URL
        QString startDate = ui->dateEdit->text(); // Replace with actual start date
        QString endDate = ui->dateEdit_2->text();   // Replace with actual end date

        // Construct the URL
        url = "https://api.reliefweb.int/v1/disasters?appname=rwint-user-0&profile=list&preset=latest&slim=1&query%5Bvalue%5D=date.event%3A%5B" +
              startDate + "+TO+" + endDate + "%7D+AND+country.id%3A" + tara + "+AND+type.id%3A" + dezastru + "+AND+status%3A" + status + "&query%5Boperator%5D=AND";
    }

    // Make the API request
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Second_Page::onNetworkReply);

    QNetworkRequest request(url);
    manager->get(request);
}

void Second_Page::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonObject jsonObj = jsonDoc.object();
        QJsonArray jsonArray = jsonObj["data"].toArray();

        QString text;
        for (const QJsonValue &value : jsonArray) {
            QString name = value.toObject()["fields"].toObject()["name"].toString();
            text +=name + "\n\n";
        }
        ui->textEdit->setText(text);
    } else {
        ui->textEdit->setText("Request failed: " + reply->errorString());
    }

    reply->deleteLater();
}

void Second_Page::listen_for_alerts()
{
    if (this->socket != nullptr && this->socket->isOpen())
    {
        QByteArray notification = this->socket->readAll();
        QString response_str = QString::fromStdString(notification.toStdString());
        if(response_str.length() > 0 && response_str != "3\u0000S.O.S")
        {
            qDebug()<<response_str;
            QMessageBox::information(this,"Alert",response_str,QMessageBox::Ok);
            ui->noteHistory->append(response_str);


            this->socket->readAll();
        }
    }
}

void Second_Page::on_pushButton_4_clicked()
{
    QString to_send = "3*S.O.S";
    socket->write(to_send.toStdString().c_str());
    this->socket->flush();
}

