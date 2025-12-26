import os
import tkinter as tk
from PIL import Image, ImageTk

# Configuration
IMAGE_DIR = "."  # Looks in the current folder
EXTENSIONS = (".jpg", ".jpeg", ".JPG", ".JPEG")

class ImageRenamer:
    def __init__(self, root):
        self.root = root
        self.root.title("Quick Image Renamer")
        
        # Get list of jpg files
        self.files = [f for f in os.listdir(IMAGE_DIR) if f.endswith(EXTENSIONS)]
        self.index = 0

        if not self.files:
            print("No JPG files found in this folder.")
            self.root.destroy()
            return

        # UI Setup
        self.image_label = tk.Label(root)
        self.image_label.pack(pady=10)

        self.filename_label = tk.Label(root, text="", font=("Arial", 10, "italic"))
        self.filename_label.pack()

        self.entry = tk.Entry(root, font=("Arial", 14), width=40)
        self.entry.pack(pady=10, padx=20)
        self.entry.bind("<Return>", self.rename_file) # Press Enter to rename
        self.entry.focus_set()

        self.status_label = tk.Label(root, text=f"File 1 of {len(self.files)}")
        self.status_label.pack(side="bottom", pady=5)

        self.load_image()

    def load_image(self):
        if self.index < len(self.files):
            file_path = os.path.join(IMAGE_DIR, self.files[self.index])
            
            # Open and resize image to fit screen comfortably
            img = Image.open(file_path)
            img.thumbnail((800, 600)) 
            self.photo = ImageTk.PhotoImage(img)
            
            self.image_label.config(image=self.photo)
            self.filename_label.config(text=f"Current file: {self.files[self.index]}")
            self.status_label.config(text=f"File {self.index + 1} of {len(self.files)}")
            self.entry.delete(0, tk.END)
        else:
            print("All files renamed!")
            self.root.destroy()

    def rename_file(self, event):
        new_name = self.entry.get().strip()
        if new_name:
            old_path = os.path.join(IMAGE_DIR, self.files[self.index])
            # Automatically adds .jpg
            new_path = os.path.join(IMAGE_DIR, new_name + ".jpg")
            
            try:
                os.rename(old_path, new_path)
                self.index += 1
                self.load_image()
            except Exception as e:
                print(f"Error renaming: {e}")
        else:
            # If you press Enter with no text, it just skips the file
            self.index += 1
            self.load_image()

if __name__ == "__main__":
    root = tk.Tk()
    app = ImageRenamer(root)
    root.mainloop()