import pdf_edit as edit
import tkinter as tk
from tkinter import filedialog
from tkinter import font as tkfont
import os
from PyPDF2 import PdfFileReader, PdfFileWriter
from docx import Document
from docx.shared import Inches
import sys
import comptypes.client

class Application (tk.Tk):
    def __init__(self):
        self.resizable(0,0)
        self.geometry("350x280")
        self.title("Edit PDFs")
        self.create_widgets()

    def create_widgwts(self):
        


