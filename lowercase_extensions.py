#!/usr/bin/env python3
"""
Lowercase Extensions Script
Renames all image files to have lowercase extensions (.JPG → .jpg, .PNG → .png)
Updates references in README.md and other markdown files
"""

import os
import sys
import platform
from pathlib import Path

# Image extensions to process
IMAGE_EXTENSIONS = {'.jpg', '.jpeg', '.png', '.gif', '.bmp', '.svg', '.webp'}

def is_windows():
    """Check if running on Windows"""
    return platform.system() == 'Windows'

def find_images_with_uppercase_extensions(directory):
    """Find all image files with uppercase extensions"""
    images_to_rename = []
    
    for root, dirs, files in os.walk(directory):
        for filename in files:
            file_path = Path(root) / filename
            ext = file_path.suffix.lower()
            
            # Check if it's an image extension and if the actual extension has uppercase
            if ext in IMAGE_EXTENSIONS and file_path.suffix != ext:
                images_to_rename.append(file_path)
    
    return images_to_rename

def rename_image_file(file_path):
    """Rename a single image file to lowercase extension"""
    new_name = file_path.stem + file_path.suffix.lower()
    new_path = file_path.parent / new_name
    
    try:
        # Check if we're renaming to the same name (case-insensitive match)
        if file_path.name.lower() == new_name.lower() and file_path.name != new_name:
            # This is a case-only change on a case-insensitive filesystem
            # Use a temporary name to force the rename
            temp_name = file_path.stem + "_TEMP_RENAME_" + file_path.suffix
            temp_path = file_path.parent / temp_name
            
            # First rename to temp
            os.rename(str(file_path), str(temp_path))
            print(f"  → Step 1: {file_path.name} → {temp_name}")
            
            # Then rename to final name
            os.rename(str(temp_path), str(new_path))
            print(f"  → Step 2: {temp_name} → {new_name}")
            print(f"✅ Renamed: {file_path.name} → {new_name}")
            return (file_path.name, new_name)
        else:
            # Direct rename (different names or case-sensitive filesystem)
            if new_path.exists():
                print(f"⏭️  Skipped: {file_path.name} (lowercase version already exists)")
                return None
            
            os.rename(str(file_path), str(new_path))
            print(f"✅ Renamed: {file_path.name} → {new_name}")
            return (file_path.name, new_name)
            
    except Exception as e:
        print(f"❌ Error renaming {file_path.name}: {e}")
        import traceback
        traceback.print_exc()
        return None

def update_markdown_files(directory, renamed_files):
    """Update all markdown files with new image names"""
    if not renamed_files:
        return 0
    
    updated_count = 0
    md_files = list(Path(directory).rglob('*.md'))
    
    print(f"\n📝 Found {len(md_files)} markdown file(s) to check...")
    
    for md_file in md_files:
        try:
            content = md_file.read_text(encoding='utf-8')
            original_content = content
            changes_made = []
            
            # Replace each renamed file reference
            for old_name, new_name in renamed_files:
                if old_name in content:
                    content = content.replace(old_name, new_name)
                    changes_made.append(f"{old_name} → {new_name}")
            
            # Write back if changed
            if content != original_content:
                md_file.write_text(content, encoding='utf-8')
                print(f"✅ Updated: {md_file.relative_to(directory)}")
                for change in changes_made:
                    print(f"   • {change}")
                updated_count += 1
        
        except Exception as e:
            print(f"❌ Error updating {md_file.name}: {e}")
    
    return updated_count

def main():
    """Main function"""
    print("=" * 70)
    print("  LOWERCASE IMAGE EXTENSIONS SCRIPT")
    print("=" * 70)
    print()
    
    # Get the directory where the script is located
    script_dir = Path(__file__).parent.resolve()
    print(f"📁 Working directory: {script_dir}")
    print(f"💻 Operating System: {platform.system()}")
    print()
    
    # Step 1: Find all images with uppercase extensions
    print("STEP 1: Finding images with uppercase extensions...")
    print("-" * 70)
    images_to_rename = find_images_with_uppercase_extensions(script_dir)
    
    if not images_to_rename:
        print("✨ No images found with uppercase extensions!")
        print("=" * 70)
        return
    
    print(f"Found {len(images_to_rename)} image(s) to rename:\n")
    for img in images_to_rename:
        rel_path = img.relative_to(script_dir)
        print(f"  • {rel_path} (extension: {img.suffix})")
    
    # Step 2: Rename the files
    print(f"\n{'=' * 70}")
    print("STEP 2: Renaming image files...")
    print("-" * 70)
    
    renamed_files = []
    for img_path in images_to_rename:
        result = rename_image_file(img_path)
        if result:
            renamed_files.append(result)
    
    # Step 3: Update markdown files
    print(f"\n{'=' * 70}")
    print("STEP 3: Updating markdown files...")
    print("-" * 70)
    
    if renamed_files:
        updated_count = update_markdown_files(script_dir, renamed_files)
    else:
        print("ℹ️  No files were renamed, skipping markdown update")
        updated_count = 0
    
    # Print summary
    print(f"\n{'=' * 70}")
    print("📊 SUMMARY:")
    print(f"   ✅ Image files renamed: {len(renamed_files)}")
    print(f"   📝 Markdown files updated: {updated_count}")
    print("=" * 70)
    
    if renamed_files:
        print("\n📋 Renamed files:")
        for old, new in renamed_files:
            print(f"   {old} → {new}")
    
    print("\n💡 TIP: After running this script, commit and push the changes to GitHub.")

if __name__ == "__main__":
    try:
        main()
        print("\n✨ Script complete!")
    except KeyboardInterrupt:
        print("\n\n⚠️  Script cancelled by user")
        sys.exit(1)
    except Exception as e:
        print(f"\n❌ An unexpected error occurred: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
    
    input("\nPress Enter to exit...")