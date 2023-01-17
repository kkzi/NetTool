import os
import subprocess

qt_dir = 'C:/Qt/4.3.5'
moc_to_dir = 'out/moc'
moc = f'{qt_dir}/bin/moc.exe'

def moc_file(path):
    filename = path.split('/')[-1]
    subprocess.run([f'{moc}', path, '-o', f'{moc_to_dir}/moc_{filename}.cpp'])

def moc_files(arr):
    [moc_file(i) for i in arr]

if not os.path.exists(moc_to_dir):
    os.mkdir(moc_to_dir)

moc_files([
    'Src/SideBar.h', 
    'Src/Form.h',
    'Src/RecvWidget.h',
    'Src/SendWidget.h',
    'Src/NetworkTask.h',
    'Src/RemoteHostWidget.h',
])
