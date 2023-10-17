#!/usr/bin/env python3
from http.server import HTTPServer, SimpleHTTPRequestHandler, test
import sys

class CustomHandler (SimpleHTTPRequestHandler):
	def end_headers (self):
		self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
		self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
		SimpleHTTPRequestHandler.end_headers(self)

if __name__ == '__main__':
	test(CustomHandler, HTTPServer, port=8000, bind='localhost')
