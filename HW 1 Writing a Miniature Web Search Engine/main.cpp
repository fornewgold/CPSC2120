/*
  CPSC 2120
  Xi Zhao
  9/18/2014
*/

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct Page
{
	string key, word;
	double weight, new_weight;
	int num_link, num_word;
	int hash_ID;
	Page *next;
	Page(string k, string w, double wt, double new_wt, int n_l, int n_w, int ID, Page *n)
	{
		key = k;
		word = w;
		weight = wt;
		new_weight = new_wt;
		num_link = n_l;
		num_word = n_w;
		hash_ID = ID;
		next = n;
	}
	Page(Page* p)
	{
		key = p->key;
		word = p->word;
		weight = p->weight;
		new_weight = p->new_weight;
		num_link = p->num_link;
		num_word = p->num_word;
		hash_ID = p->hash_ID;
		next = p->next;
	}
	Page()
	{
		key = "";
		word = "";
		weight = 0;
		new_weight = 0;
		num_link = 0;
		num_word = 0;
		hash_ID = -1;
		next = NULL;
	}
};

struct Link
{
	string key, page;
	int hash_ID;
	Link *next;
	Link(string k, string p, int ID, Link *n) { key = k; page = p; hash_ID = ID; next = n; }
	Link() { key = ""; page = ""; hash_ID = -1; next = NULL; }
};

struct Word
{
	string key, page;
	int hash_ID;
	Word *next;
	Word(string k, string p, int ID, Word *n) { key = k; page = p; hash_ID = ID; next = n; }
	Word() { key = "", page = ""; hash_ID = -1; next = NULL; }
};

/* Return a hash for the string s in the range 0..table_size-1 */
int hashString(const string s, const int table_size)
{
	unsigned int i, h = 0;
	for (i = 0; i < s.length(); i++)
		h = (h * 2917 + (unsigned int)s[i]) % table_size;
	return h;
}

/* Implement the Google Pagerank Algorithm */
void rankPage(const int n, Page** pages, Link** links)
{
	cout << "Ranking pages..." << endl;
	
	// Initialize weight with 1 / N
	for (int i = 0; i < n; i++)
	{
		Page *p = pages[i];
		while (p != NULL)
		{
			p->weight = 1.0 / n;
			p = p->next;
		}
	}

	// Repeat 50 times
	for (int k = 0; k < 50; k++)
	{
		// For each page i, set new_weight[i] = 0.1 / N
		for (int i = 0; i < n; i++)
		{
			Page *p = pages[i];
			while (p != NULL)
			{
				p->new_weight = 0.1 / n;
				p = p->next;
			}
		}

		// For each page i
		for (int i = 0; i < n; i++)
		{
			Page *p = pages[i];
			while (p != NULL)
			{
				// Count total number of linked pages, t
				int t = 0;
				Link *cl = links[p->hash_ID];	// Hash page to linked links
				while (cl != NULL)
				{
					if (cl->page == p->key)
					{
						Page *cp = pages[cl->hash_ID];
						while (cp != NULL)
						{
							if (cp->key == cl->key)
								t++; // Count if the link found in linked list for pages
							cp = cp->next;
						}
					}
					cl = cl->next;
				}
				
				// For each page j (of t total) to which i links		
				Link *it = links[p->hash_ID]; 	// Hash page to linked links
				while (it != NULL)
				{
					if (it->page == p->key)
					{
						Page *q = pages[it->hash_ID];
						while (q != NULL)
						{
							if (q->key == it->key)
								// Increase new_weight[j] by 0.9 * weight[i] / t
								//q->new_weight += (0.9 * p->weight / p->num_link); // Redistribution weight to all links
								q->new_weight += (0.9 * p->weight / t); // Redistribution weight to links in the page list only
							q = q->next;
						}
					}
					it = it->next;
				}
				p = p->next;
			}
		}

		// For each page i, set weight[i] = new_weight[i]
		for (int i = 0; i < n; i++)
		{
			Page *p = pages[i];
			while (p != NULL)
			{
				p->weight = p->new_weight;
				p = p->next;
			}
		}
	}
	
	cout << "done!" << endl;
}

/* Creating invert index for words */
void createInvertIndex(const int num_NEWPAGE, const int num_word, Page** pages, Word** words, Page** inv_idx)
{
	cout << "Creating invert index..." << endl;
	
	for (int i = 0; i < num_word; i++)
		inv_idx[i] = NULL;
		
	for (int i = 0; i < num_NEWPAGE; i++)
	{
		Word *w = words[i];
		while (w != NULL)
		{
			Page *p = pages[w->hash_ID];
			while (p !=NULL)
			{						
				if (p->key == w->page)
				{
					int h = hashString(w->key, num_word);
					Page *q = new Page(p);
					q->next = inv_idx[h];
					inv_idx[h] = q;					
				}
				p = p->next;
			}
			w = w->next;
		}
	}
	
	cout << "done!" << endl;
}

/* Search typed word */
void searchWord(const int size, Page** index)
{
	string keyword;
	
	cout << "Please enter keyword to search:" << endl;
	while (cin >> keyword) 
	{
		cout << "The pages contain '" << keyword << "' are:" << endl;;

		// Switch keyword to lowercase
		for (int i = 0; i < keyword.length(); i++)
			if ((keyword[i] >= 65) && (keyword[i] <= 90))
				keyword[i] += 32;
				
		int count = 0;
		Page *p = index[hashString(keyword, size)];
		while (p != NULL)
		{
			cout << (int) (p->weight * 10000000) << " " << p->key << endl;
			p = p->next;
			count++;
		}
		cout << count << " pages found!" << endl;
	}
}

int main(void)
{
	ifstream fin;
	string s;
	int num_NEWPAGE = 0, num_link = 0, num_word = 0, h;

	// Read input file and find the number of NEWPAGE
	fin.open("webpages.txt");
	while (fin >> s)
	{
		if (s == "NEWPAGE")
			num_NEWPAGE++;
		else
		{
			string url_str = "http://";
			if (s.find(url_str) == 0)
				num_link++;
			else
				num_word++;
		}
	}
	fin.close();
	cout << "Total number of pages is " << num_NEWPAGE << endl;
	cout << "Total number of links is " << num_link << endl;
	cout << "Total number of words is " << num_word << endl;
	
	// Allocate space for linked list for pages
	cout << "Allocating space..." << endl;

	Page **pages = new Page *[num_NEWPAGE];
	for (int i = 0; i < num_NEWPAGE; i++)
		pages[i] = NULL;

	// Allocate space for linked list for links
	Link **links = new Link *[num_NEWPAGE];
	for (int i = 0; i < num_NEWPAGE; i++)
		links[i] = NULL;

	// Allocate space for linked list for words
	Word **words = new Word *[num_NEWPAGE];
	for (int i = 0; i < num_NEWPAGE; i++)
		words[i] = NULL;

	cout << "done!" << endl;

	// Read input file and insert each string into the appropriate linked list based on the content
	cout << "Reading file..." << endl;
	
	fin.open("webpages.txt");
	while (fin >> s)
	{
		if (s == "NEWPAGE")
		{
			fin >> s;
			h = hashString(s, num_NEWPAGE); // Hash the NEWPAGE link to linked lists
			pages[h] = new Page(s, "", 0, 0, 0, 0, h, pages[h]); // Insert pages into page list
		}
		else
		{
			string url_str = "http://";
			if (s.find(url_str) == 0)
			{
				links[h] = new Link(s, pages[h]->key, hashString(s, num_NEWPAGE), links[h]); // Insert links into link list
				pages[h]->num_link++;
			}
			else
			{
				words[h] = new Word(s, pages[h]->key, h, words[h]); // Insert words into word list
				pages[h]->num_word++;
			}
		}
	}
	fin.close();
	
	cout << "done!" << endl;
	
	// Rank pages
	rankPage(num_NEWPAGE, pages, links);
	
	
	double sum = 0;
	for (int i = 0; i < num_NEWPAGE; i++)
	{
		Page *p = pages[i];
		while (p != NULL)
		{
			sum += p->weight;
			p = p->next;
		}
	}
	cout << "Sum of final weight is: " << sum << endl; // If sum = 1, no weight for links out of the page list
	

	// Creating invert index for words
	Page **inv_idx = new Page *[num_word];
	createInvertIndex(num_NEWPAGE, num_word, pages, words, inv_idx);

	// Search typed word
	searchWord(num_word, inv_idx);
	
	return 0;
}
