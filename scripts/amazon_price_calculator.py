#!/usr/bin/env python3
"""
Extract Amazon product links from README and calculate total price
"""

import re
import requests
from bs4 import BeautifulSoup
import time

def extract_amazon_links(filepath):
    """
    Extract all Amazon product links from markdown file
    """
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Find Amazon product URLs (both /dp/ and /gp/product/)
    amazon_pattern = r'https://www\.amazon\.com/(?:dp|gp/product)/([A-Z0-9]{10})'
    matches = re.findall(amazon_pattern, content)
    
    # Deduplicate and create full URLs
    product_ids = list(set(matches))
    urls = [f"https://www.amazon.com/dp/{pid}" for pid in product_ids]
    
    return urls

def get_amazon_price(url):
    """
    Scrape price from Amazon product page
    Returns: (product_name, price_float, currency_symbol)
    """
    try:
        headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
            'Accept-Language': 'en-US,en;q=0.5',
            'Accept-Encoding': 'gzip, deflate',
            'Connection': 'keep-alive',
        }
        
        response = requests.get(url, headers=headers, timeout=10)
        
        if response.status_code != 200:
            return (None, None, None, f"HTTP {response.status_code}")
        
        soup = BeautifulSoup(response.content, 'html.parser')
        
        # Try to get product name
        title_elem = soup.find('span', id='productTitle')
        product_name = title_elem.get_text().strip() if title_elem else "Unknown Product"
        product_name = product_name[:60] + "..." if len(product_name) > 60 else product_name
        
        # Try multiple price selectors (Amazon changes these frequently)
        price_selectors = [
            ('span', {'class': 'a-price-whole'}),
            ('span', {'class': 'a-offscreen'}),
            ('span', {'id': 'priceblock_ourprice'}),
            ('span', {'id': 'priceblock_dealprice'}),
            ('span', {'class': 'a-price aok-align-center reinventPricePriceToPayMargin priceToPay'}),
        ]
        
        price_text = None
        for tag, attrs in price_selectors:
            price_elem = soup.find(tag, attrs)
            if price_elem:
                price_text = price_elem.get_text()
                break
        
        if not price_text:
            return (product_name, None, None, "Price not found")
        
        # Parse price (handle formats like "$12.99", "12.99", "$1,234.56")
        price_clean = re.sub(r'[^\d.]', '', price_text)
        
        try:
            price = float(price_clean)
            currency = '$'  # Assume USD for .com
            return (product_name, price, currency, None)
        except ValueError:
            return (product_name, None, None, f"Could not parse: {price_text}")
            
    except Exception as e:
        return (None, None, None, str(e))

def calculate_amazon_total(filepath):
    """
    Calculate total price of all Amazon products in README
    """
    print(f"📖 Reading {filepath}...\n")
    
    urls = extract_amazon_links(filepath)
    
    if not urls:
        print("❌ No Amazon product links found!")
        return
    
    print(f"🔍 Found {len(urls)} unique Amazon product(s)\n")
    print("=" * 80)
    
    results = []
    total = 0.0
    errors = []
    
    for i, url in enumerate(urls, 1):
        print(f"\n[{i}/{len(urls)}] Fetching: {url}")
        
        product_name, price, currency, error = get_amazon_price(url)
        
        if error:
            print(f"  ❌ {error}")
            errors.append((url, product_name or "Unknown", error))
        elif price:
            print(f"  ✅ {product_name}")
            print(f"     Price: {currency}{price:.2f}")
            results.append((product_name, price, currency))
            total += price
        
        # Be nice to Amazon
        time.sleep(2)
    
    print("\n" + "=" * 80)
    print("\n💰 PRICE SUMMARY\n")
    
    if results:
        print("Products found:\n")
        for i, (name, price, currency) in enumerate(results, 1):
            print(f"{i:2d}. {name[:50]:50s} {currency}{price:>8.2f}")
        
        print(f"\n{'─' * 80}")
        print(f"{'TOTAL:':50s} ${total:>8.2f}")
        print(f"{'─' * 80}")
        
        # Rough estimates
        print(f"\n📊 Estimates:")
        print(f"   With Tax (8%):  ${total * 1.08:.2f}")
        print(f"   With Tax (10%): ${total * 1.10:.2f}")
    
    if errors:
        print(f"\n⚠️  Could not fetch {len(errors)} product(s):")
        for url, name, error in errors:
            print(f"\n  • {name}")
            print(f"    {url}")
            print(f"    Error: {error}")
    
    if not results and not errors:
        print("❌ No prices could be retrieved")

if __name__ == "__main__":
    import sys
    
    if len(sys.argv) != 2:
        print("Usage: python amazon_price_calculator.py <path_to_readme.md>")
        sys.exit(1)
    
    # Check if BeautifulSoup is installed
    try:
        from bs4 import BeautifulSoup
    except ImportError:
        print("❌ Error: BeautifulSoup4 not installed")
        print("Install with: pip install beautifulsoup4")
        sys.exit(1)
    
    filepath = sys.argv[1]
    calculate_amazon_total(filepath)

