import QtQuick 2.14
import QtTest 1.14
import HttpClient 1.0

TestCase {
    id: root

    name: "[CuteMockServer]"

    function test_listenHttpFailure() {
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.5; CuteMockServer { }", root)

        verify(mockServer !== null)
        compare(mockServer.listen(8080, true), true)
        compare(mockServer.listen(8080), false)
    }

    function test_listenSecureHttpFailure() {
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.5; CuteMockServer { }", root)

        verify(mockServer !== null)
        compare(mockServer.listen(4443), true)
        compare(mockServer.listen(4443, true), false)
    }

    function test_listenHttpAndConnect() {
        var client = createTemporaryQmlObject("import HttpClient 1.0; HttpClient { }", root)
        var connectSpy = createTemporaryQmlObject("import QtTest 1.14; SignalSpy { }", root)
        connectSpy.target = client
        connectSpy.signalName = "success"
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.5; CuteMockServer { }", root)
        verify(mockServer !== null)
        compare(mockServer.listen(8080), true)

        client.get("http://localhost:8080/")
        connectSpy.wait(150)
        compare(connectSpy.count, 1)
    }

    function test_listenSecureHttpAndConnect() {
        var client = createTemporaryQmlObject("import HttpClient 1.0; HttpClient { }", root)
        var connectSpy = createTemporaryQmlObject("import QtTest 1.14; SignalSpy { }", root)
        connectSpy.target = client
        connectSpy.signalName = "success"
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.5; CuteMockServer { }", root)
        verify(mockServer !== null)
        compare(mockServer.listen(4443, true), true)

        client.get("https://localhost:4443/")
        connectSpy.wait(150)
        compare(connectSpy.count, 1)
    }

    function test_listenHttpAndSecureHttpAndConnect() {
        var client = createTemporaryQmlObject("import HttpClient 1.0; HttpClient { }", root)
        var connectSpy = createTemporaryQmlObject("import QtTest 1.14; SignalSpy { }", root)
        connectSpy.target = client
        connectSpy.signalName = "success"
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.5; CuteMockServer { }", root)
        verify(mockServer !== null)
        compare(mockServer.listen(4443, true), true)

        client.get("https://localhost:4443/")
        connectSpy.wait(150)
        compare(connectSpy.count, 1)
    }

    function test_nonexistentHttpRoute() {
        var client = createTemporaryQmlObject("import HttpClient 1.0; HttpClient { }", root)
        var connectSpy = createTemporaryQmlObject("import QtTest 1.14; SignalSpy { }", root)
        connectSpy.target = client
        connectSpy.signalName = "success"
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.5; CuteMockServer { }", root)
        mockServer.setHttpRoute("GET", "/", 200, "", "");
        verify(mockServer !== null)
        compare(mockServer.listen(8080), true)

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
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.5; CuteMockServer { }", root)
        mockServer.setHttpRoute("DELETE", "/delete/route", 200, "application/json", "{\"deleted\": \"true\"}");
        verify(mockServer !== null)
        compare(mockServer.listen(8080), true)

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
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.5; CuteMockServer { }", root)
        mockServer.setHttpRoute("PUT", "/wrong/router", 201, "", "");
        verify(mockServer !== null)
        compare(mockServer.listen(4443, true), true)

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
        var mockServer = createTemporaryQmlObject("import CuteMockServer 0.5; CuteMockServer { }", root)
        var testImage = createTemporaryQmlObject("import CuteMockServer 0.5; CuteFile { }", root)
        compare(testImage.openFile(":/test_image.png"), true)
        mockServer.setHttpRoute("POST", "/post/route", 202, "image/png", testImage.data);
        verify(mockServer !== null)
        compare(mockServer.listen(4443, true), true)

        client.post("https://localhost:4443/post/route")
        connectSpy.wait(150)
        compare(connectSpy.count, 1)
        var args = connectSpy.signalArguments[0]
        compare(args[0], 202)                       // statusCode
        compare(args[1], "image/png")               // contentType
        compare(args[2], 1544)                      // contentLength
        compare(args[3].byteLength, 1544)           // content
    }
}
