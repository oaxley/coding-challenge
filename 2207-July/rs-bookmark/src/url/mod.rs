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
 * @brief	Main URL module
 */

//----- imports


//----- modules
mod create;
mod read;
mod update;
mod delete;


//----- functions

/* retrieve a string from an Option<&String>
 *
 * Args:
 *      option: the Option<&String> to get the value from
 *
 * Returns:
 *      The String if it's defined in the option, otherwise ""
 */
fn get_string(option: Option<&String>) -> String {
    if let Some(value) = option {
        return value.to_string();
    } else {
        return String::from("");
    }
}

/* create a string from a vector of tags
 *
 * Args:
 *      tags: a vector of String
 *
 * Returns:
 *      A string with all the element of the vector concatenate with ','.
 *      Returns an empty string if no tags are defined.
 */
fn get_tags(tags: Vec<&String>) -> String {
    let mut value = String::from("");
    for tag in tags {
        if value.is_empty() {
            value = tag.to_string();
        } else {
            value = value + ",";
            value = value + &tag.to_string();
        }
    }

    value
}


//----- exports
pub use create::create as create;
pub use read::read as read;
pub use update::update as update;
pub use delete::delete as delete;
