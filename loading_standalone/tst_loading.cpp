/*
 * Copyright (C) 2009 Holger Hans Peter Freyther
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <QtTest/QtTest>

#include <qwebframe.h>
#include <qwebview.h>
#include <qpainter.h>

#include "QNetworkReplyHandler.h"
#include "WebCore_classes.h"

using namespace WebCore;

class tst_Loading : public QObject
{
    Q_OBJECT

public:

private Q_SLOTS:
    void loadAll();

private:
    QWebView* m_view;
    QWebPage* m_page;
};

QNetworkAccessManager* globalManager = new QNetworkAccessManager;

static int jobsToDo = 0;

class Client : public ResourceHandleClient {
public:
    void didReceiveResponse(ResourceHandle* handle, const ResourceResponse&) { }
    void didReceiveData(ResourceHandle* handle, const char*, int, int) { }
    void didFinishLoading(ResourceHandle* handle) { --jobsToDo; }
    void willSendRequest(ResourceHandle* handle, const ResourceRequest& req, const ResourceResponse& resp) { qWarning() << __PRETTY_FUNCTION__ << handle->request().url(); }
};

static void createJob(ResourceHandleClient* client, const QUrl& url)
{
    ++jobsToDo;
    ResourceRequest req(url);

    // yes these will be leaked...
    ResourceHandle* handle = new ResourceHandle(client, req);
    QNetworkReplyHandler* handler = new QNetworkReplyHandler(handle, QNetworkReplyHandler::LoadNormal);
}

void tst_Loading::loadAll()
{

    QBENCHMARK {
        Client client;

        createJob(&client, QUrl("http://en.wikipedia.org/wiki/Maxwell_equations"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/shared.css?239az2"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/shared.css"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/commonPrint.css?239az2"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/commonPrint.css"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/monobook/main.css?239az2"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/monobook/main.css"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/chick/main.css?239az2"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/chick/main.css"));
        createJob(&client, QUrl("http://upload.wikimedia.org/centralnotice/wikipedia/en/centralnotice.js?239az2"));
        createJob(&client, QUrl("http://upload.wikimedia.org/centralnotice/wikipedia/en/centralnotice.js"));
        createJob(&client, QUrl("http://en.wikipedia.org/w/index.php?title=MediaWiki:Common.css&usemsgcache=yes&ctype=text%2Fcss&smaxage=2678400&action=raw&maxage=2678400"));
        createJob(&client, QUrl("http://en.wikipedia.org/w/index.php"));
        createJob(&client, QUrl("http://en.wikipedia.org/w/index.php?title=MediaWiki:Print.css&usemsgcache=yes&ctype=text%2Fcss&smaxage=2678400&action=raw&maxage=2678400"));
        createJob(&client, QUrl("http://en.wikipedia.org/w/index.php?title=MediaWiki:Handheld.css&usemsgcache=yes&ctype=text%2Fcss&smaxage=2678400&action=raw&maxage=2678400"));
        createJob(&client, QUrl("http://en.wikipedia.org/w/index.php?title=MediaWiki:Monobook.css&usemsgcache=yes&ctype=text%2Fcss&smaxage=2678400&action=raw&maxage=2678400"));
        createJob(&client, QUrl("http://en.wikipedia.org/w/index.php?title=-&action=raw&maxage=2678400&gen=css"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/wikibits.js?urid=239az2"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/wikibits.js"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/ajax.js?urid=239az2"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/ajax.js"));
        createJob(&client, QUrl("http://en.wikipedia.org/w/index.php?title=-&action=raw&gen=js&useskin=monobook&urid=239az2"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/mwsuggest.js?urid=239az2"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/mwsuggest.js"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/monobook/headbg.jpg"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/monobook/external.png"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/monobook/bullet.gif"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/images/magnify-clip.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/wikipedia/commons/thumb/a/a6/Solenoid.svg/180px-Solenoid.svg.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/wikipedia/commons/thumb/5/51/Magnetic_core.jpg/180px-Magnetic_core.jpg"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/monobook/document.png"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/monobook/lock_icon.gif"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/monobook/user.gif"));
        createJob(&client, QUrl("http://en.wikipedia.org/skins-1.5/common/images/poweredby_mediawiki_88x31.png"));
        createJob(&client, QUrl("http://meta.wikimedia.org/w/index.php?title=MediaWiki:Wikiminiatlas.js&action=raw&ctype=text/javascript&smaxage=21600&maxage=86400"));
        createJob(&client, QUrl("http://meta.wikimedia.org/w/index.php"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/d/f/5dfc5942c917f6b36ace7773cfbe5c88.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/9/8/f/98f468f189234d748a5117254c093829.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/9/0/4/9048aa1e501c7b9e6f7f758b10c490f7.png"));
        createJob(&client, QUrl("http://en.wikipedia.org/images/wikimedia-button.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/7/6/57619c6a86c79e56ac806faf21502c90.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/9/c/a/9cab6787646062d6e658cd1e83ad468f.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/7/6/9/76994d6f5c0e0285e4b229aed959500d.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/9/a/39adeb66b53fc1be92dda9c01386c3a9.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/4/1/2/412fa35baeef0cdb663c1c4fef80c67c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/4/0/6/406b4d694a9bf931d960dbc6c83abb87.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/4/6/d46b953d19ec8ad255d2fc7ad1f4403d.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/2/0/8/208794b4735713017a1f643c3532b90d.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/a/7/5a7f8e7e20e5579970b5e6a39cdd3b0c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/1/c/8/1c88050c83555c51feb62402559d6cae.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/2/f/1/2f144e06df6aae683a5f0de2f4ab8237.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/9/f/e9fc53c99b1222ccfc1013146c3c0243.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/0/c/1/0c162a264a721d78cb4fd1dc04ed80ab.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/2/b/2/2b2c35626c28b0f43e03d10af0400cb6.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/5/5/3554c6443080faa1ac733057b4506fe2.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/3/3/d33f7e439256001686806fdd6c7a5603.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/1/0/4/104e907b2b3bfabd0a13f1749e03a1ea.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/0/9/f0925836443f166e63b1931de41e3cb4.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/5/2/352e6e8aca588923cf005a23994568a4.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/6/3/7/637e9aa8f9fc5c99c48feb86b1377aa0.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/0/0/a/00a60ebc2590b45c2151ec6b12e3fd39.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/9/8/8/98885d3a4380905aae1fa922cfebbd0e.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/4/9/34983b2048fce0c1a78d2380effad0cb.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/8/f/d/8fdd129a0ce6360fe6073c4d2ea6ff89.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/4/e/d/4ede6b55c75fdc76e3d957e44ae92b07.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/1/1/6/1164e59b735f64b45ef5a2afd42ecacd.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/a/b/5/ab50c15340becb687b8c45e35a01a207.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/a/7/1/a7183eb2c881c885f2eb6f87a9a7fe6c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/0/e/e0e12c971016cfe92211454a4c59de7f.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/4/a/f/4afbdcfd4b1689a8d40b3b4b72c2504e.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/e/d/3eda2f1156ca24f03f85d7751fd70210.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/3/2/3323de6eb9d8ab8c6042f55ca1383d01.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/b/9/fb956cb5a3b9da78b5c6a44cac82faeb.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/d/0/3d06df63af794fac3660cd0a1a110cfa.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/6/5/e65e205d10a67380612560c9dff33de2.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/a/3/c/a3c26bba57368d9ae6742068a23720cf.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/c/2/b/c2bc71132ddc9180edb1abf19ba69252.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/4/1/9/41968d7938b8145f26e1d196abc77144.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/a/3/da3623c683c463e210845c6d757af4dd.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/wikipedia/commons/2/23/Icons-mini-file_acrobat.gif"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/9/d/f9d43f3fdfd0e1bd61a61f139e99267c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/1/c/7/1c7f10dc9ecf8d7045be8f19979e3823.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/e/b/5eb237ccb8c2716d347ab313cad7918e.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/b/7/5b711f39bd91a89cc7b4cb4ec592eaa7.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/1/1/a/11aa0e631ed8c4b1be0b8bba19d08c68.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/2/b/1/2b1d0440bb057183eba9dd12a5b23330.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/c/a/7/ca7fd72914756606853d4e6771b13ccc.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/0/e/50e8bc7f9b72bc593c998dbc41ed5eb6.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/1/a/3/1a3f2d60ec59fbe26a76f2995a64413a.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/7/4/1/741dd921f51cbde4870df80510919b75.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/b/8/eb8e03b942c5f551d3e4b2c3f1d522a4.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/a/e/3/ae35f5350252e2b75e6eaf30553c2ff7.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/6/0/d60ab072bfe725f0247cd3567724936a.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/9/2/5/92555f9439ef4a54fcd65bd62f44f4ee.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/f/1/3f1521dbf60aeb9e9026a676de625d01.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/wikipedia/commons/thumb/4/4c/Wikisource-logo.svg/50px-Wikisource-logo.svg.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/2/9/b/29bb4c095ea3dd6ddfa37f81a68ad953.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/6/b/7/6b767392e32fd1dc6b95c651b28cd324.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/b/6/d/b6d2c966ed2c1f7dbc397f62ad9a3395.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/wikipedia/commons/thumb/e/e2/Polarization_and_magnetization.svg/300px-Polarization_and_magnetization.svg.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/7/1/3/713369ba07ea4fbec5e73dd3230ed086.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/9/9/399fb1a618c0788278bc256841c636d5.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/c/6/fc63adfa1732515411a29fc898d35e70.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/c/1/d/c1d6084109fd190af9806626f74c714f.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/6/8/2/6820c4b3ecbcc5448daba8201c6320d4.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/9/3/f9373c1985e2c08b5175148b229e8c77.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/7/1/4/714c2314b67b4282b7b10e945df6ec7d.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/6/4/d642ee6a77e2b9e0160bfbf8eac39a2b.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/6/8/b/68b64b0230b21ecc7f0bb00dcb4a51fc.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/4/2/f42ff73555c78900f1c7755e1c4a9a3c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/6/a/0/6a0f7daefe4e6c37fb0417f3d81dceee.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/a/6/d/a6d7995542d66894024c752599c78d25.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/0/d/d/0ddb62418fea4baed3f64360308938f8.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/1/9/e196d92ffe2ef5f4837aefffa9f4c1ae.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/8/c/58c2b8b14b73d2bafeaaafb80b4a5491.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/b/e/3/be3332f81add1c939ecf9ee92d6cc738.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/2/f/9/2f9c5e8c60afb3cfde1e9ea1619cf9c3.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/a/c/d/acdbca818fd5285411ea9fd48f87a05d.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/0/7/c/07c9309ea117d3418c513500195f902c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/8/d/e8d3e21a0b0646076770b6eda3a6a61c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/2/c/d/2cd3561c61742d7abaf0d6b3d599bdf3.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/d/8/dd89480478cc4d090d6705b85ef6e270.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/3/2/5325fa7f00960a7a6487663663a33a59.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/2/8/2/282b37baa375d6dd6811c1cab30a60aa.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/1/3/f/13fa029ce3dbd18fb247bef77a8b0e60.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/8/a/d8aee925110fda33395e45036a61bb70.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/c/1/2/c126e2afbcf5f65647a0ed69b6f74866.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/6/e/d6e04318e279d9810f7b976f416a88d8.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/2/e/c/2ec9ba8a9b6e259e0aa10196304b06a0.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/c/d/0/cd06a9e5fdb2eff6384680b2bc7ef912.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/f/0/ef074e0554ec1ed8c547d17bf5e0b9bf.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/f/9/5f93651130be1f2671fcb622c77f8f06.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/7/0/5/7052f302b7234944c9f4b2a5da46b8e4.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/b/4/fb4c8eed0e054512b32be15554a128eb.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/4/d/2/4d2d1823e064a6ccefcf6919d9d75aa3.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/a/e/3ae176cfd1af85a0b692a584193cc73b.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/7/1/5/715fdcb32a8a7cdf59571006dd4f819e.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/a/5/8/a586eaf12e965431a094e32e21b0c31c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/1/8/e18145d90d95c7b0839acdefbdcca7ea.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/2/7/52780f761cb723058494da66c9beef0c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/3/f/f3fb08adaa66e81c8a830e8efe4824cd.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/1/6/c/16c6abcd001c113b16487e33f6d7163c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/7/3/d738d844f1e9d9e53c596411a55b7a31.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/9/8/f98c8024db68e152f96a645328875fb5.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/2/b/d2b812e8d55ceecc9d5f263f6220d6eb.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/4/4/e441e16270e1f0c4c33f8a746e9ddb44.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/0/3/f03f83e0a4e7e73ae1199a73c767b45e.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/8/7/e/87e5d4d82ce84c2dba92a67ab2a85aea.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/4/d/54d8e9f31e563aa9f8e5dec570d3cbb6.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/0/2/f021561ea64f5ddb945ded7e27c5fb49.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/a/0/5a006a440682e53daef14dca39bbc22c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/6/f/e6f91a8576a1134e0d9a85ba9c45f7b6.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/a/3/4/a34c6421fe1b16dac4eedb6dbfa65e2f.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/5/3/3538eb9c84efdcbd130c4c953781cfdb.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/8/c/5/8c5d71d25ccf822fcab3825cf68c9506.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/4/f/0/4f0218b8b3a67e2ffb8a098621a273ed.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/7/0/e/70e82f9a275c5f3057a7f926ff4db000.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/2/9/e/29ed508c553583e1477fc6a3681158fe.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/1/e/d/1eddf3bea7144dda0929072bd3953d91.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/8/1/d818627ac1a4179c30daa965320bb696.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/3/4/e3403ab33ecba276a98dd76f68498257.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/c/7/c/c7c24a63210a2d7a68ca205e55f47391.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/c/1/3c11bc732bcfc7a177bdc017b3fc771b.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/d/0/1/d0179884952518b0f627718dd0a75f09.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/0/2/e/02ed78660e758e55520daa1d78a3dccc.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/7/0/3/7036208fae1b80ae621b28de5c4380b7.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/c/c/e/cce5392ef32a5f3bd122f069a6c5dbbd.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/3/8/e/38e0c2b27ca1e99f072a93923b4e7c70.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/b/1/7/b1779dd5fb9bce5ce1bf68e9a644f861.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/c/7/fc7aebc918618541e3d2048d423ce238.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/c/6/b/c6b02fe30ddd00571f9f3614bd97030d.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/e/c/feca97c81fc5881053946b15536dd902.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/5/e/7/5e73cb322dc2ca760795e2e1f6d9b5bb.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/e/3/fe3a83e41074834731743ab803cd4936.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/d/e/fde786a300f1b03551215edd8c7ef73c.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/e/4/0/e4024c70de0dc74a4c31c7139d388416.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/8/e/a/8ea33eb8e8e2ffc427488a3d553b9a65.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/c/d/7/cd7297b0374d844c74c07ae946b21fa1.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/f/c/2/fc22951f0cbfd234d02aabaa7f7d8358.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/1/3/1/131bf44b1029baa8dcd03c5e02cc1591.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/2/d/0/2d0b9236bf93d316edf7821a0a3d4a77.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/0/5/a/05a58720e440dc00d43b6434305ceaa3.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/9/8/d/98db8892b5949cb1441f8705bc38bcf2.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/8/8/8/8880f3d38407a0039e2eb37f98f0eb9a.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/8/1/5/815497e3328be2da714bb9d4cc59bcaf.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/b/4/b/b4b2b367a342835efcbb9067e4c36354.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/math/4/2/d/42d5b4247496a805df8099010d51db92.png"));
        createJob(&client, QUrl("http://upload.wikimedia.org/wikipedia/en/1/18/Monobook-bullet.png"));

    while (jobsToDo > 0)
        qApp->processEvents();
    }
}

QTEST_MAIN(tst_Loading)
#include "tst_loading.moc"
