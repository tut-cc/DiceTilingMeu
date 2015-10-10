module procon26.http;

import std.net.curl;
import std.format;
import std.uri;
import std.array;
import std.stdio;
import std.string;


struct RequestSpec
{
  static
  {
    string host = `testform26.procon-online.net`;
    string token = `0123456789abcdef`;
  }
}


string getProblem(uint num)
{
    string url = format("http://%s/quest%s.txt?token=%s", RequestSpec.host, num, RequestSpec.token);
    writeln(url);

    return std.net.curl.get(url).dup;
}


string postAnswer(string ans)
{
    string url = format("http://%s/answer", RequestSpec.host);

    auto app = appender!string();
    HTTP http = HTTP();

    immutable boundary = `cce6735153bf14e47e999e68bb183e70a1fa7fc89722fc1efdf03a917340`;   // 適当な文字列
    http.addRequestHeader("Content-Type", "multipart/form-data; boundary=" ~ boundary);

    app.put("--"); app.put(boundary); app.put("\r\n");
    app.put(`Content-Disposition: form-data; name="token"`); app.put("\r\n");
    app.put("\r\n");
    app.put(RequestSpec.token); app.put("\r\n");
    app.put("--"); app.put(boundary); app.put("\r\n");
    app.put(`Content-Disposition: form-data; name="answer"; filename="answer.txt"`); app.put("\r\n");
    app.put(`Content-Type: text/plain`); app.put("\r\n");
    app.put("\r\n");
    app.put(ans); app.put("\r\n");
    app.put("--"); app.put(boundary); app.put("--\r\n");

    return std.net.curl.post(url, app.data, http).dup;
}
