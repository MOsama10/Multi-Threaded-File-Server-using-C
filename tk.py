import subprocess

import tkinter as tk



def upload():

    filename = filename_entry.get()

    client_process.stdin.write("upload\n")

    client_process.stdin.write(filename + "\n")

    client_process.stdin.flush()



def download():

    filename = filename_entry.get()

    client_process.stdin.write("download\n")

    client_process.stdin.write(filename + "\n")

    client_process.stdin.flush()



def rename():

    old_filename = old_filename_entry.get()

    new_filename = new_filename_entry.get()

    client_process.stdin.write("rename\n")

    client_process.stdin.write(old_filename + "\n")

    client_process.stdin.write(new_filename + "\n")

    client_process.stdin.flush()



def exit_client():

    client_process.stdin.write("exit\n")

    client_process.stdin.flush()

    root.destroy()



# Run the client executable

client_process = subprocess.Popen(["./client"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)



# Create the Tkinter GUI

root = tk.Tk()

root.title("Client Interface")



# Upload

upload_label = tk.Label(root, text="Upload")

upload_label.grid(row=0, column=0, padx=10, pady=5)



filename_label = tk.Label(root, text="Filename:")

filename_label.grid(row=1, column=0, padx=10, pady=5)



filename_entry = tk.Entry(root)

filename_entry.grid(row=1, column=1, padx=10, pady=5)



upload_button = tk.Button(root, text="Upload", command=upload)

upload_button.grid(row=2, column=0, columnspan=2, padx=10, pady=5)



# Download

download_label = tk.Label(root, text="Download")

download_label.grid(row=3, column=0, padx=10, pady=5)



download_button = tk.Button(root, text="Download", command=download)

download_button.grid(row=4, column=0, columnspan=2, padx=10, pady=5)



# Rename

rename_label = tk.Label(root, text="Rename")

rename_label.grid(row=5, column=0, padx=10, pady=5)



old_filename_label = tk.Label(root, text="Old Filename:")

old_filename_label.grid(row=6, column=0, padx=10, pady=5)



old_filename_entry = tk.Entry(root)

old_filename_entry.grid(row=6, column=1, padx=10, pady=5)



new_filename_label = tk.Label(root, text="New Filename:")

new_filename_label.grid(row=7, column=0, padx=10, pady=5)



new_filename_entry = tk.Entry(root)

new_filename_entry.grid(row=7, column=1, padx=10, pady=5)



rename_button = tk.Button(root, text="Rename", command=rename)

rename_button.grid(row=8, column=0, columnspan=2, padx=10, pady=5)



# Exit

exit_button = tk.Button(root, text="Exit", command=exit_client)

exit_button.grid(row=9, column=0, columnspan=2, padx=10, pady=5)



root.mainloop()

