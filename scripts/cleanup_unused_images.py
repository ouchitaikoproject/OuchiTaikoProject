#!/usr/bin/env python3
"""
Image Cleanup Script for OuchiTaiko Project
Finds and optionally deletes unused images in the images/Pictures directory
"""

import os
import re
from pathlib import Path

# Configuration
PROJECT_ROOT = Path(__file__).parent.parent
IMAGES_DIR = PROJECT_ROOT / "images" / "Pictures"
README_FILE = PROJECT_ROOT / "README.md"
INDEX_FILE = PROJECT_ROOT / "index.html"

def find_image_references_in_file(filepath):
    """Extract all image references from a file."""
    references = set()
    
    if not filepath.exists():
        print(f"Warning: {filepath} not found")
        return references
    
    content = filepath.read_text(encoding='utf-8')
    
    # Pattern 1: Markdown images - ![alt](path)
    md_pattern = r'!\[.*?\]\((.*?)\)'
    for match in re.finditer(md_pattern, content):
        img_path = match.group(1)
        # Extract just the filename
        filename = os.path.basename(img_path)
        references.add(filename)
    
    # Pattern 2: HTML img tags - <img src="path">
    html_pattern = r'<img[^>]+src=["\']([^"\']+)["\']'
    for match in re.finditer(html_pattern, content, re.IGNORECASE):
        img_path = match.group(1)
        filename = os.path.basename(img_path)
        references.add(filename)
    
    # Pattern 3: Direct path references (like in your README)
    path_pattern = r'images/Pictures/([\w\-. ]+\.(jpg|png|gif|jpeg|webp))'
    for match in re.finditer(path_pattern, content, re.IGNORECASE):
        filename = match.group(1)
        references.add(filename)
    
    return references

def get_all_images():
    """Get all image files in the images/Pictures directory."""
    if not IMAGES_DIR.exists():
        print(f"Error: Images directory not found: {IMAGES_DIR}")
        return set()
    
    image_extensions = {'.jpg', '.jpeg', '.png', '.gif', '.webp'}
    images = set()
    
    for item in IMAGES_DIR.iterdir():
        if item.is_file() and item.suffix.lower() in image_extensions:
            images.add(item.name)
    
    return images

def main():
    print("=" * 70)
    print("OuchiTaiko Project - Image Cleanup Script")
    print("=" * 70)
    print()
    
    # Find all image references
    print("Scanning files for image references...")
    readme_refs = find_image_references_in_file(README_FILE)
    index_refs = find_image_references_in_file(INDEX_FILE)
    all_refs = readme_refs | index_refs
    
    print(f"  README.md: {len(readme_refs)} unique images referenced")
    print(f"  index.html: {len(index_refs)} unique images referenced")
    print(f"  Total unique: {len(all_refs)} images")
    print()
    
    # Find all images in directory
    print("Scanning images directory...")
    all_images = get_all_images()
    print(f"  Found: {len(all_images)} image files")
    print()
    
    # Find unused images
    unused_images = all_images - all_refs
    
    if not unused_images:
        print("✓ No unused images found! Everything is referenced.")
        return
    
    # Display results
    print("=" * 70)
    print(f"UNUSED IMAGES ({len(unused_images)} files)")
    print("=" * 70)
    
    # Sort for easier reading
    for img in sorted(unused_images):
        img_path = IMAGES_DIR / img
        size_kb = img_path.stat().st_size / 1024
        print(f"  {img:<40} ({size_kb:>7.1f} KB)")
    
    print()
    total_size_kb = sum((IMAGES_DIR / img).stat().st_size for img in unused_images) / 1024
    total_size_mb = total_size_kb / 1024
    print(f"Total wasted space: {total_size_mb:.2f} MB ({total_size_kb:.1f} KB)")
    print()
    
    # Ask for confirmation to delete
    print("=" * 70)
    response = input("Do you want to DELETE these unused images? (yes/no): ").strip().lower()
    
    if response == 'yes':
        print()
        print("Deleting unused images...")
        deleted_count = 0
        for img in unused_images:
            img_path = IMAGES_DIR / img
            try:
                img_path.unlink()
                print(f"  ✓ Deleted: {img}")
                deleted_count += 1
            except Exception as e:
                print(f"  ✗ Error deleting {img}: {e}")
        
        print()
        print(f"✓ Successfully deleted {deleted_count} of {len(unused_images)} images")
        print(f"✓ Freed up {total_size_mb:.2f} MB of space")
    else:
        print()
        print("✓ No files were deleted. Run this script again when ready.")
    
    print()
    print("=" * 70)

if __name__ == "__main__":
    main()
