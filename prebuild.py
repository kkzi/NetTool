import os
import subprocess

qt_dir = 'C:/Qt/4.3.5'
moc = f'{qt_dir}/bin/moc.exe'

def moc_file(dir, path):
    filename = path.split('/')[-1]
    subprocess.run([f'{moc}', path, '-o', f'{dir}/moc_{filename}.cpp'])

def moc_files(dir, arr):
    if not os.path.exists(dir):
        os.mkdir(dir)
    [moc_file(dir, i) for i in arr]


moc_files('out/src', [
    'Src/SideBar.h', 
    'Src/Form.h',
    'Src/RecvWidget.h',
    'Src/SendWidget.h',
    'Src/RemoteHostWidget.h',
])

moc_files('out/core', [
    'Core/NetworkTask.h',
    'Core/TcpServerTask.h',
])
