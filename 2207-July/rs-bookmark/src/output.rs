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
 * @brief	Module to handle the different output format
 */

//----- imports
use serde::Serialize;


//----- functions
/* Print the content of T with the selected format
 *
 * Args:
 *      values: the value / container to print
 *      format: the selected format
 *
 * Returns:
 *      None
 */
pub fn serialize<T>(values: &T, format: &str) -> ()
    where T: Serialize
{
    match format.to_lowercase().as_str() {
        "json" => {
            let output = serde_json::to_string_pretty(&values).unwrap();
            println!("{}", output);
        },

        "yaml" => {
            let output = serde_yaml::to_string(&values).unwrap();
            println!("{}", output);
        }

        _ => {
            eprintln!("Error: unsupported format {}", format);
        }

    }
}
