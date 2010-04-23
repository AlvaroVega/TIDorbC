SUBDIRS = include .

AM_CPPFLAGS = \
  -I${top_srcdir}/source/TIDSocket/tidsocket/include \
  -I${top_srcdir}/source/TIDThr/tidthr/include \
  -I${top_srcdir}/source/TIDSocket/tidsocket/tidsocket/include
#  -I${srcdir}/include

lib_LTLIBRARIES = libtidsocket.la

libtidsocket_la_LIBADD = \
  ${top_builddir}/source/TIDThr/tidthr/tidthr/libtidthr.la \
  $(SSL_LIB)

libtidsocket_la_SOURCES = \
        DatagramChannel.C \
        DatagramPacket.C \
        DatagramSocket.C \
        DatagramSocketImpl.C \
        Exception.C \
        InetAddress.C \
        InetSocketAddress.C \
        InputStream.C \
        MulticastSocket.C \
        NetworkInterface.C \
        OutputStream.C \
        PlainSocketImpl.C \
        SelectableChannel.C \
        SelectionKey.C \
        Selector.C \
        ServerSocket.C \
        ServerSocketChannel.C \
        Socket.C \
        SocketChannel.C \
        SocketImpl.C \
	SSLContext.C \
	SSLInputStream.C \
	SSLOutputStream.C \
	SSLServerSocket.C \
	SSLServerSocketFactory.C \
	SSLSession.C \
	SSLSessionContext.C \
	SSLSocket.C \
	SSLSocketFactory.C


libtidsocket_la_LDFLAGS = \
	-avoid-version
