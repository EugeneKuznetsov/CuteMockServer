import QtQuick 2.14
import QtTest 1.14
import HttpClient 1.0

TestCase {
    id: root

    name: "[CuteMockServer]"

    function test_listenHttpFailure() {
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.4; CuteMockServer { }", root)

        verify(mockServer !== null)
        compare(mockServer.listenHttps(8080), true)
        compare(mockServer.listenHttp(8080), false)
    }

    function test_listenSecureHttpFailure() {
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.4; CuteMockServer { }", root)

        verify(mockServer !== null)
        compare(mockServer.listenHttp(4443), true)
        compare(mockServer.listenHttps(4443), false)
    }

    function test_listenHttpAndConnect() {
        var client = createTemporaryQmlObject("import HttpClient 1.0; HttpClient { }", root)
        var connectSpy = createTemporaryQmlObject("import QtTest 1.14; SignalSpy { }", root)
        connectSpy.target = client
        connectSpy.signalName = "success"
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.4; CuteMockServer { }", root)
        verify(mockServer !== null)
        compare(mockServer.listenHttp(8080), true)

        client.get("http://localhost:8080/")
        connectSpy.wait(150)
        compare(connectSpy.count, 1)
    }

    function test_listenSecureHttpAndConnect() {
        var client = createTemporaryQmlObject("import HttpClient 1.0; HttpClient { }", root)
        var connectSpy = createTemporaryQmlObject("import QtTest 1.14; SignalSpy { }", root)
        connectSpy.target = client
        connectSpy.signalName = "success"
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.4; CuteMockServer { }", root)
        verify(mockServer !== null)
        compare(mockServer.listenHttps(4443), true)

        client.get("https://localhost:4443/")
        connectSpy.wait(150)
        compare(connectSpy.count, 1)
    }

    function test_listenHttpAndSecureHttpAndConnect() {
        var client = createTemporaryQmlObject("import HttpClient 1.0; HttpClient { }", root)
        var connectSpy = createTemporaryQmlObject("import QtTest 1.14; SignalSpy { }", root)
        connectSpy.target = client
        connectSpy.signalName = "success"
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.4; CuteMockServer { }", root)
        verify(mockServer !== null)
        compare(mockServer.listenHttps(4443), true)

        client.get("https://localhost:4443/")
        connectSpy.wait(150)
        compare(connectSpy.count, 1)
    }

    function test_nonexistentHttpRoute() {
        var client = createTemporaryQmlObject("import HttpClient 1.0; HttpClient { }", root)
        var connectSpy = createTemporaryQmlObject("import QtTest 1.14; SignalSpy { }", root)
        connectSpy.target = client
        connectSpy.signalName = "success"
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.4; CuteMockServer { }", root)
        mockServer.setHttpRoute("GET", "/", 200, "", "");
        verify(mockServer !== null)
        compare(mockServer.listenHttp(8080), true)

        client.get("http://localhost:8080/wrong/route")
        connectSpy.wait(150)
        compare(connectSpy.count, 1)
        var args = connectSpy.signalArguments[0]
        compare(args[0], 404)   // statusCode
        compare(args[1], "")    // contentType
        compare(args[2], 0)     // contentLength
        compare(args[3], "")    // content
    }

    function test_existingHttpRoute() {
        var client = createTemporaryQmlObject("import HttpClient 1.0; HttpClient { }", root)
        var connectSpy = createTemporaryQmlObject("import QtTest 1.14; SignalSpy { }", root)
        connectSpy.target = client
        connectSpy.signalName = "success"
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.4; CuteMockServer { }", root)
        mockServer.setHttpRoute("DELETE", "/delete/route", 200, "application/json", "{\"deleted\": \"true\"}");
        verify(mockServer !== null)
        compare(mockServer.listenHttp(8080), true)

        client.deleteResource("http://localhost:8080/delete/route")
        connectSpy.wait(150)
        compare(connectSpy.count, 1)
        var args = connectSpy.signalArguments[0]
        compare(args[0], 200)                   // statusCode
        compare(args[1], "application/json")    // contentType
        compare(args[2], 19)                    // contentLength
        compare(JSON.parse(args[3]), {"deleted": "true"})   // content
    }

    function test_nonexistentSecureHttpRoute() {
        var client = createTemporaryQmlObject("import HttpClient 1.0; HttpClient { }", root)
        var connectSpy = createTemporaryQmlObject("import QtTest 1.14; SignalSpy { }", root)
        connectSpy.target = client
        connectSpy.signalName = "success"
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.4; CuteMockServer { }", root)
        mockServer.setHttpRoute("PUT", "/wrong/router", 201, "", "");
        verify(mockServer !== null)
        compare(mockServer.listenHttps(4443), true)

        client.put("https://localhost:4443/wrong/route")
        connectSpy.wait(150)
        compare(connectSpy.count, 1)
        var args = connectSpy.signalArguments[0]
        compare(args[0], 404)   // statusCode
        compare(args[1], "")    // contentType
        compare(args[2], 0)     // contentLength
        compare(args[3], "")    // content
    }

    function test_existingSecureHttpRoute() {
        var client = createTemporaryQmlObject("import HttpClient 1.0; HttpClient { }", root)
        var connectSpy = createTemporaryQmlObject("import QtTest 1.14; SignalSpy { }", root)
        connectSpy.target = client
        connectSpy.signalName = "success"
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.4; CuteMockServer { }", root)
        mockServer.setHttpRoute("POST", "/post/route", 202, "text/html", "Hello cute QML Mock Client");
        verify(mockServer !== null)
        compare(mockServer.listenHttps(4443), true)

        client.post("https://localhost:4443/post/route")
        connectSpy.wait(150)
        compare(connectSpy.count, 1)
        var args = connectSpy.signalArguments[0]
        compare(args[0], 202)                           // statusCode
        compare(args[1], "text/html")                   // contentType
        compare(args[2], 26)                            // contentLength
        compare(args[3], "Hello cute QML Mock Client")  // content
    }
}
