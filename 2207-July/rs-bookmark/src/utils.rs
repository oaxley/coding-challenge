/* -*- coding: utf-8 -*-
 * vim: set ft=rust
 *
 * This source file is subject to the Apache License 2.0
 * that is bundled with this package in the file LICENSE.txt.
 * It is also available through the Internet at this address:
 * https://opensource.org/licenses/Apache-2.0
 *
 * @author	Sebastien LEGRAND
 * @license	Apache License 2.0
 *
 * @brief	Helper functions
 */

//----- imports
use std::env;
use std::fmt;
use clap::ArgMatches;


//----- globals
const ENV_STORE_NAME: &str = "DATABASE";


//----- structures
/* Parameters structure
 *
 * Args:
 *      id, store, url, description, tags, format, regex
 */
pub struct Params<'a>(
    pub Option<&'a String>,         // 0: ID
    pub Option<&'a String>,         // 1: Store
    pub Option<&'a String>,         // 2: URL
    pub Option<&'a String>,         // 3: Description
    pub Vec<&'a String>,            // 4: Tags
    pub Option<&'a String>,         // 5: Format
    pub Option<&'a String>,         // 6: Regex
);

impl fmt::Display for Params<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "ID\t= {:?}\nStore\t= {:?}\nURL\t= {:?}\nDescr\t= {:?}\nTags\t= {:?}\nFormat\t= {:?}\nRegex\t= {:?}",
            self.0, self.1, self.2, self.3, self.4, self.5, self.6)
    }
}

//----- functions

/* retrieve the store name
 *
 * Args:
 *      store: the Option<&String> with the store name
 *
 * Returns:
 *      the store name as a String or an empty string if it cannot be found
 */
pub fn get_store_name(store: Option<&String>) -> String {
    if let Some(value) = store {
        return value.to_string();
    } else {
        match getenv_store() {
            Some(value) => return value,
            None => return String::from("")
        }
    }
}

/* retrieve the database name from the environment variable
 *
 * Args:
 *
 * Returns:
 *      An Option<String> with the value of the environment variable
 */
fn getenv_store() -> Option<String> {
    match env::var(ENV_STORE_NAME) {
        Ok(value) => return Some(value),
        Err(_) => return None
    }
}

/* lookup for the parameter and gets its value
 *
 * Args:
 *      matches: the ArgMatches from the command line
 *      name: the parameter to look for
 *
 * Returns:
 *      The value of the parameter or None if it's not present
 */
fn get_string_value<'a>(matches: &'a ArgMatches, name: &str) -> Option<&'a String> {
    match matches.try_get_one::<String>(&name) {
        Ok(value) => value,
        Err(_) => None
    }
}

/* retrieve the values from the arg matches and create a "Named Tuple" out of it
 *
 * Args:
 *      matches: the matches from Clap parsing
 *
 * Returns:
 *      a named tuple according to URLParams
 */
pub fn retrieve_params(matches: &ArgMatches) -> Params {

    // if a value is not defined, fall back to the default value of None
    let id: Option<&String> = get_string_value(matches, "ID");
    let url = get_string_value(matches, "url");
    let descr = get_string_value(matches, "descr");
    let store = get_string_value(matches, "store");
    let format = get_string_value(matches, "format");
    let regex = get_string_value(matches, "regex");

    let tags: Vec<&String>;
    match matches.try_get_many::<String>("tags") {
        Ok(value) => tags = value.into_iter().flatten().collect::<Vec<_>>(),
        Err(_) => tags = vec![]
    }

    Params {
        0: id,        // ID
        1: store,       // Store
        2: url,         // URL
        3: descr,       // Description
        4: tags,        // Tags
        5: format,      // Format
        6: regex        // Regex
    }
}
