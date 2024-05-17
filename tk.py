import subprocess

import tkinter as tk



def upload():

    filename = upload_filename_entry.get()

    client_process.stdin.write("upload\n")

    client_process.stdin.write(filename + "\n")

    client_process.stdin.flush()

    reset_entries()



def download():

    filename = download_filename_entry.get()

    client_process.stdin.write("download\n")

    client_process.stdin.write(filename + "\n")

    client_process.stdin.flush()

    reset_entries()



def rename():

    old_filename = old_filename_entry.get()

    new_filename = new_filename_entry.get()

    client_process.stdin.write("rename\n")

    client_process.stdin.write(old_filename + "\n")

    client_process.stdin.write(new_filename + "\n")

    client_process.stdin.flush()

    reset_entries()



def exit_client():

    client_process.stdin.write("exit\n")

    client_process.stdin.flush()

    root.destroy()



def reset_entries():

    upload_filename_entry.delete(0, tk.END)

    download_filename_entry.delete(0, tk.END)

    old_filename_entry.delete(0, tk.END)

    new_filename_entry.delete(0, tk.END)



# Run the client executable

client_process = subprocess.Popen(["./client"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)



# Create the Tkinter GUI

root = tk.Tk()

root.title("Client Interface")



# Upload

upload_label = tk.Label(root, text="Upload")

upload_label.grid(row=0, column=0, padx=10, pady=5)



upload_filename_label = tk.Label(root, text="File Path:")

upload_filename_label.grid(row=1, column=0, padx=10, pady=5)



upload_filename_entry = tk.Entry(root)

upload_filename_entry.grid(row=1, column=1, padx=10, pady=5)



upload_button = tk.Button(root, text="Upload", command=upload)

upload_button.grid(row=2, column=0, columnspan=2, padx=10, pady=5)



# Download

download_label = tk.Label(root, text="Download")

download_label.grid(row=3, column=0, padx=10, pady=5)



download_filename_label = tk.Label(root, text="File Name:")

download_filename_label.grid(row=4, column=0, padx=10, pady=5)



download_filename_entry = tk.Entry(root)

download_filename_entry.grid(row=4, column=1, padx=10, pady=5)



download_button = tk.Button(root, text="Download", command=download)

download_button.grid(row=5, column=0, columnspan=2, padx=10, pady=5)



# Rename

rename_label = tk.Label(root, text="Rename")

rename_label.grid(row=6, column=0, padx=10, pady=5)



old_filename_label = tk.Label(root, text="Old Filename:")

old_filename_label.grid(row=7, column=0, padx=10, pady=5)



old_filename_entry = tk.Entry(root)

old_filename_entry.grid(row=7, column=1, padx=10, pady=5)



new_filename_label = tk.Label(root, text="New Filename:")

new_filename_label.grid(row=8, column=0, padx=10, pady=5)



new_filename_entry = tk.Entry(root)

new_filename_entry.grid(row=8, column=1, padx=10, pady=5)



rename_button = tk.Button(root, text="Rename", command=rename)

rename_button.grid(row=9, column=0, columnspan=2, padx=10, pady=5)



# Exit

exit_button = tk.Button(root, text="Exit", command=exit_client)

exit_button.grid(row=10, column=0, columnspan=2, padx=10, pady=5)



root.mainloop()
