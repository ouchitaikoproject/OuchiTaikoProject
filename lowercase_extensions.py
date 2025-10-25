#!/usr/bin/env python3
"""
Lowercase Extensions Script
Renames all image files to have lowercase extensions (.JPG → .jpg, .PNG → .png)
Also updates references in README.md files
"""

import os
import re
from pathlib import Path

# Image extensions to process
IMAGE_EXTENSIONS = {'.JPG', '.PNG', '.JPEG', '.GIF', '.BMP', '.SVG', '.WEBP'}

def lowercase_file_extensions(directory=None):
    """
    Rename all image files in directory to have lowercase extensions
    
    Args:
        directory: Path to search (default: current directory)
    """
    if directory is None:
        directory = Path(__file__).parent
    else:
        directory = Path(directory)
    
    print(f"🔍 Searching for images in: {directory.absolute()}\n")
    
    renamed_files = []
    skipped_files = []
    
    # Find all files with uppercase extensions
    for file_path in directory.rglob('*'):
        if not file_path.is_file():
            continue
        
        # Check if extension needs lowercasing
        if file_path.suffix.upper() in IMAGE_EXTENSIONS and file_path.suffix != file_path.suffix.lower():
            new_name = file_path.stem + file_path.suffix.lower()
            new_path = file_path.parent / new_name
            
            # Check if target already exists
            if new_path.exists():
                print(f"⏭️  Skipped: {file_path.name} (lowercase version already exists)")
                skipped_files.append((str(file_path.name), new_name))
                continue
            
            # Rename the file
            try:
                file_path.rename(new_path)
                print(f"✅ Renamed: {file_path.name} → {new_name}")
                renamed_files.append((str(file_path.name), new_name))
            except Exception as e:
                print(f"❌ Error renaming {file_path.name}: {e}")
    
    return renamed_files, skipped_files

def update_markdown_files(directory=None, renamed_files=None):
    """
    Update references in markdown files to use lowercase extensions
    
    Args:
        directory: Path to search for .md files (default: current directory)
        renamed_files: List of (old_name, new_name) tuples
    """
    if directory is None:
        directory = Path(__file__).parent
    else:
        directory = Path(directory)
    
    if not renamed_files:
        return
    
    print("\n" + "="*60)
    print("📝 Updating markdown files...\n")
    
    updated_count = 0
    
    # Find all markdown files
    for md_file in directory.rglob('*.md'):
        try:
            # Read the file
            content = md_file.read_text(encoding='utf-8')
            original_content = content
            
            # Replace each renamed file reference
            for old_name, new_name in renamed_files:
                # Replace in image paths
                content = content.replace(old_name, new_name)
            
            # Write back if changed
            if content != original_content:
                md_file.write_text(content, encoding='utf-8')
                print(f"✅ Updated: {md_file.name}")
                updated_count += 1
            
        except Exception as e:
            print(f"❌ Error updating {md_file.name}: {e}")
    
    if updated_count == 0:
        print("ℹ️  No markdown files needed updating")
    
    return updated_count

def main():
    """Main function"""
    print("="*60)
    print("  Lowercase Image Extensions Script")
    print("="*60)
    print()
    
    # Get the directory where the script is located
    script_dir = Path(__file__).parent
    
    # Step 1: Rename image files
    print("STEP 1: Renaming image files...")
    print("-"*60)
    renamed_files, skipped_files = lowercase_file_extensions(script_dir)
    
    # Step 2: Update markdown files
    if renamed_files:
        updated_count = update_markdown_files(script_dir, renamed_files)
    else:
        print("\nℹ️  No files were renamed, skipping markdown update")
        updated_count = 0
    
    # Print summary
    print("\n" + "="*60)
    print("📊 SUMMARY:")
    print(f"   ✅ Image files renamed: {len(renamed_files)}")
    print(f"   ⏭️  Image files skipped: {len(skipped_files)}")
    print(f"   📝 Markdown files updated: {updated_count}")
    print("="*60)
    
    if renamed_files:
        print("\n📋 Files renamed:")
        for old, new in renamed_files:
            print(f"   • {old} → {new}")

if __name__ == "__main__":
    try:
        main()
        print("\n✨ Script complete!")
    except Exception as e:
        print(f"\n❌ An error occurred: {e}")
    
    input("\nPress Enter to exit...")
