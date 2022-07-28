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
 * @brief	Module to read/print an URL
 */

//----- imports
use crate::database as database;
use crate::utils as utils;
use crate::url::get_string;


//----- functions
/* read a URL from the database and print it
 *
 * Args:
 *      The parameters for the function extracted from the command line
 *
 * Returns:
 *      true if we found the element in the database
 */
pub fn read(params: utils::Params) -> bool {

    // retrieve the database name either from arguments or environment
    let db_name: String = utils::get_store_name(params.1);
    if db_name.is_empty() {
        eprintln!("Error: unable to determine the database name!");
        return false;
    }


    // execute the query
    match database::connect(&db_name) {
        Some(db) => {
            // query and ID
            let query: String = String::from("SELECT * FROM bookmarks WHERE id=(?1)");
            let id: String = get_string(params.0);

            // SQL statement
            match db.prepare(&query) {

                Ok(mut stmt) => {
                    // retrieve the record
                    match stmt.query_row((&id,), |row| {
                        Ok(
                            database::Bookmark {
                                id: row.get(0).unwrap(),
                                url: row.get(1).unwrap(),
                                description: row.get(2).unwrap(),
                                tags: row.get(3).unwrap()
                            }
                        )})
                    {
                        Ok(bookmark) => {
                            println!("{}", bookmark);
                            return true
                        },

                        Err(_) => {
                            eprintln!("Error: unable to retrieve the bookmark with ID {}", id);
                            return false
                        }
                    }
                },

                Err(_) => {
                    eprintln!("Error: unable to prepare the SQL statement");
                    return false
                }
            }
        },

        None => {
            eprintln!("Error: unable to get a connection to database");
            return false
        }
    }
}
