#!/usr/bin/env python3
"""
Interactive Amazon price entry tool
Opens each Amazon link in browser and prompts for price entry
"""

import re
import webbrowser
import time

def extract_amazon_links_with_context(filepath):
    """
    Extract Amazon links with surrounding context from markdown
    """
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Find Amazon links with markdown link text
    markdown_links = re.findall(r'\[([^\]]+)\]\((https://www\.amazon\.com/[^)]+)\)', content)
    
    products = []
    for link_text, url in markdown_links:
        # Extract product ID
        match = re.search(r'/dp/([A-Z0-9]{10})', url)
        if match:
            product_id = match.group(1)
            products.append({
                'name': link_text,
                'url': url,
                'id': product_id
            })
    
    # Remove duplicates based on product ID
    seen_ids = set()
    unique_products = []
    for p in products:
        if p['id'] not in seen_ids:
            seen_ids.add(p['id'])
            unique_products.append(p)
    
    return unique_products

def interactive_price_entry(filepath):
    """
    Interactive tool to enter prices for Amazon products
    """
    print("=" * 80)
    print("Amazon Price Entry Tool".center(80))
    print("=" * 80)
    print("\nInstructions:")
    print("  • Each Amazon link will open in your browser")
    print("  • Enter the price you see (just the number, e.g., 12.99)")
    print("  • Press ENTER to skip a product")
    print("  • Type 'quit' to exit early")
    print("\n" + "=" * 80 + "\n")
    
    products = extract_amazon_links_with_context(filepath)
    
    if not products:
        print("❌ No Amazon product links found in README!")
        return
    
    print(f"Found {len(products)} unique Amazon product(s)\n")
    input("Press ENTER to start...")
    print()
    
    results = []
    
    for i, product in enumerate(products, 1):
        print("\n" + "─" * 80)
        print(f"\nProduct {i}/{len(products)}")
        print(f"Name: {product['name']}")
        print(f"URL:  {product['url'][:60]}...")
        print("\nOpening in browser...")
        
        # Open link in browser
        webbrowser.open(product['url'])
        
        # Wait a moment for page to load
        time.sleep(1)
        
        while True:
            price_input = input(f"\nEnter price (or ENTER to skip, 'quit' to exit): $").strip()
            
            if price_input.lower() == 'quit':
                print("\n⚠️  Exiting early...")
                break
            
            if price_input == '':
                print("  ⏭️  Skipped")
                break
            
            # Try to parse price
            try:
                # Remove any non-numeric characters except decimal point
                price_clean = re.sub(r'[^\d.]', '', price_input)
                price = float(price_clean)
                
                if price <= 0:
                    print("  ⚠️  Price must be greater than 0. Try again.")
                    continue
                
                results.append({
                    'name': product['name'],
                    'url': product['url'],
                    'price': price
                })
                print(f"  ✅ Saved: ${price:.2f}")
                break
                
            except ValueError:
                print(f"  ⚠️  Invalid price format. Try again (e.g., 12.99)")
                continue
        
        if price_input.lower() == 'quit':
            break
    
    # Display results
    print("\n" + "=" * 80)
    print("\n💰 PRICE SUMMARY\n")
    
    if not results:
        print("No prices entered.")
        return
    
    print(f"{'Product':<50} {'Price':>10}")
    print("─" * 80)
    
    total = 0
    for item in results:
        name = item['name'][:48]
        price = item['price']
        print(f"{name:<50} ${price:>9.2f}")
        total += price
    
    print("─" * 80)
    print(f"{'TOTAL':<50} ${total:>9.2f}")
    print("=" * 80)
    
    # Tax estimates
    print(f"\n📊 Estimates:")
    print(f"   Subtotal:       ${total:.2f}")
    print(f"   With 7% tax:    ${total * 1.07:.2f}")
    print(f"   With 8% tax:    ${total * 1.08:.2f}")
    print(f"   With 10% tax:   ${total * 1.10:.2f}")
    
    # Save to file
    save = input("\n\nSave results to file? (y/n): ").strip().lower()
    if save == 'y':
        output_file = "amazon_prices_" + time.strftime("%Y%m%d_%H%M%S") + ".txt"
        
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("Amazon Product Prices - OuchiTaiko Project\n")
            f.write("=" * 80 + "\n\n")
            f.write(f"{'Product':<50} {'Price':>10}\n")
            f.write("─" * 80 + "\n")
            
            for item in results:
                f.write(f"{item['name']:<50} ${item['price']:>9.2f}\n")
            
            f.write("─" * 80 + "\n")
            f.write(f"{'TOTAL':<50} ${total:>9.2f}\n")
            f.write("=" * 80 + "\n\n")
            f.write("Estimates:\n")
            f.write(f"  Subtotal:       ${total:.2f}\n")
            f.write(f"  With 7% tax:    ${total * 1.07:.2f}\n")
            f.write(f"  With 8% tax:    ${total * 1.08:.2f}\n")
            f.write(f"  With 10% tax:   ${total * 1.10:.2f}\n")
        
        print(f"\n✅ Saved to: {output_file}")

if __name__ == "__main__":
    import sys
    
    if len(sys.argv) != 2:
        print("Usage: python manual_price_entry.py <path_to_readme.md>")
        sys.exit(1)
    
    filepath = sys.argv[1]
    
    try:
        interactive_price_entry(filepath)
    except KeyboardInterrupt:
        print("\n\n⚠️  Interrupted by user")
    
    print("\n")

