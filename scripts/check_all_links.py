#!/usr/bin/env python3
"""
Check all links in a Markdown file to verify they're working
"""

import re
import requests
from urllib.parse import urlparse
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

def check_link(url, timeout=10):
    """
    Check if a URL is accessible
    Returns: (url, status_code, is_ok, error_message)
    """
    try:
        headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36'
        }
        
        response = requests.head(url, headers=headers, allow_redirects=True, timeout=timeout)
        
        # Some servers don't like HEAD requests, try GET if HEAD fails
        if response.status_code >= 400:
            response = requests.get(url, headers=headers, allow_redirects=True, timeout=timeout, stream=True)
            response.close()
        
        is_ok = response.status_code < 400
        return (url, response.status_code, is_ok, None)
        
    except requests.exceptions.Timeout:
        return (url, None, False, "Timeout")
    except requests.exceptions.ConnectionError:
        return (url, None, False, "Connection Error")
    except requests.exceptions.TooManyRedirects:
        return (url, None, False, "Too Many Redirects")
    except Exception as e:
        return (url, None, False, str(e))

def extract_links_from_markdown(filepath):
    """
    Extract all HTTP(S) links from a Markdown file
    Returns list of unique URLs
    """
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Match markdown links [text](url) and bare URLs
    markdown_links = re.findall(r'\[([^\]]+)\]\(([^)]+)\)', content)
    bare_links = re.findall(r'(?<!\(|\[)https?://[^\s\)<>]+', content)
    
    # Combine and deduplicate
    all_links = set()
    
    # From markdown syntax [text](url)
    for text, url in markdown_links:
        if url.startswith('http'):
            all_links.add(url.split()[0])  # Remove any trailing text
    
    # Bare URLs
    for url in bare_links:
        all_links.add(url.rstrip('.,;:)'))  # Remove trailing punctuation
    
    return sorted(list(all_links))

def check_all_links(filepath, max_workers=5):
    """
    Check all links in a markdown file
    """
    print(f"📖 Reading {filepath}...\n")
    
    links = extract_links_from_markdown(filepath)
    
    if not links:
        print("❌ No links found in file!")
        return
    
    print(f"🔍 Found {len(links)} unique link(s)\n")
    print("=" * 80)
    
    results = {
        'ok': [],
        'broken': [],
        'warnings': []
    }
    
    # Check links in parallel
    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        future_to_url = {executor.submit(check_link, url): url for url in links}
        
        for i, future in enumerate(as_completed(future_to_url), 1):
            url, status_code, is_ok, error = future.result()
            
            print(f"[{i}/{len(links)}] {url[:70]}...")
            
            if is_ok:
                status_text = f"✅ OK ({status_code})"
                print(f"  {status_text}\n")
                results['ok'].append((url, status_code))
            elif status_code and 300 <= status_code < 400:
                status_text = f"⚠️  Redirect ({status_code})"
                print(f"  {status_text}\n")
                results['warnings'].append((url, status_code, "Redirect"))
            else:
                if error:
                    status_text = f"❌ BROKEN - {error}"
                else:
                    status_text = f"❌ BROKEN ({status_code})"
                print(f"  {status_text}\n")
                results['broken'].append((url, status_code, error))
            
            # Rate limiting - be nice to servers
            time.sleep(0.5)
    
    print("=" * 80)
    print("\n📊 SUMMARY\n")
    print(f"✅ Working:  {len(results['ok'])} links")
    print(f"⚠️  Warnings: {len(results['warnings'])} links")
    print(f"❌ Broken:   {len(results['broken'])} links")
    
    if results['warnings']:
        print("\n⚠️  WARNINGS:")
        for url, status, msg in results['warnings']:
            print(f"  [{status}] {url}")
    
    if results['broken']:
        print("\n❌ BROKEN LINKS:")
        for item in results['broken']:
            url = item[0]
            status = item[1] if item[1] else "N/A"
            error = item[2] if item[2] else "Unknown error"
            print(f"  [{status}] {url}")
            print(f"         {error}")
    
    if not results['broken']:
        print("\n🎉 All links are working!")
    
    return results

if __name__ == "__main__":
    import sys
    
    if len(sys.argv) != 2:
        print("Usage: python check_all_links.py <path_to_markdown_file>")
        sys.exit(1)
    
    filepath = sys.argv[1]
    check_all_links(filepath)

