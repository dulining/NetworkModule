# NetworkModule
对Qt网络请求功能的封装，使用了QNetworkReply、QNetworkRequest和QNetworkAccessManager类，将网络请求与业务逻辑模块拆分开来，同时增加了日志保存功能。 1、CLog.h/cpp：日志保存功能； 2、NetworkHelper.h/cpp：使用Qt类封装网络请求； 3、TextStreamHelper.h/cpp：使用NetworkHelper与服务器进行字符流类请求 4、PicDownHelper.h/cpp：使用NetworkHelper，进行图片下载 5、NetworkModule.h/cpp/ui：为用户界面类，使用3和4的功能进行上层业务逻辑简单实现。
